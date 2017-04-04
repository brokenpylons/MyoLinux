/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "gattclient.h"

#include <functional>

void print_address(const uint8_t *address)
{
    std::ios state(NULL);
    state.copyfmt(std::cout);

    for (int i = 0; i < 6; i++) {
        std::cout << std::hex << std::setw(2) << (int)address[i];
        if (i != 5) {
            std::cout << ":";
        }
    }
    std::cout << std::endl;
    std::cout.copyfmt(state);
}

GattClient::GattClient(const Bled112Client &client)
    : client(client)
{ }

void GattClient::discover(std::function<bool(std::int8_t, Address, Buffer)> callback)
{
    client.write(GapDiscover{GapDiscoverModeEnum::DiscoverGeneric});
    (void)client.read<GapDiscoverResponse>();

    while (true) {
        Buffer buf;
        const auto response = client.read<GapScanResponseEvent<0>>(buf);
        print_address(response.sender);

        Address address;
        std::copy(std::begin(response.sender), std::end(response.sender), std::begin(address));

        if (!callback(response.rssi, std::move(address), std::move(buf))) {
            break;
        }
    }

    client.write(GapEndProcedure{});
    (void)client.read<GapEndProcedureResponse>();
}

void GattClient::connect(const Address &address)
{
    // Check if the connection already exists
    for (std::uint8_t i = 0; i < 3; i++) { // The dongle supports 3 connections
        client.write(ConnectionGetStatus{i});
        (void)client.read<ConnectionGetStatusResponse>();
        auto status = client.read<ConnectionStatusEvent>();

        if (status.flags & ConnectionConnstatusEnum::Connected &&
                std::equal(std::begin(address), std::end(address), std::begin(status.address))) {
            connection = i;
            return;
        }
    }

    GapConnectDirect command{{}, GapAddressTypeEnum::AddressTypePublic, 6, 6, 64, 0};
    std::copy(std::begin(address), std::end(address), std::begin(command.address));
    client.write(command);

    const auto response = client.read<GapConnectDirectResponse>();
    connection = response.connection_handle;

    (void)client.read<ConnectionStatusEvent>();
    std::cout << "Connected" << std::endl;
}

void GattClient::disconnect()
{
    client.write(ConnectionDisconnect{connection});
    (void)client.read<ConnectionDisconnectResponse>();
}

void GattClient::writeAttribute(const std::uint16_t handle, const Buffer &payload)
{
    client.write(AttclientAttributeWrite<0>{connection, handle, static_cast<std::uint8_t>(payload.size())}, payload);
    (void)readResponse<AttclientAttributeWriteResponse>();
    (void)readResponse<AttclientProcedureCompletedEvent>();
}

Buffer GattClient::readAttribute(const std::uint16_t handle)
{
    client.write(AttclientReadByHandle{connection, handle});
    (void)readResponse<AttclientReadByHandleResponse>();

    Buffer data;
retry:
    const auto metadata = client.read<AttclientAttributeValueEvent<0>>(data);
    if (metadata.atthandle != handle) {
        const auto handle = metadata.atthandle;
        event_queue.emplace_back(Event{handle, std::move(data)});
        goto retry;
    }

    if (metadata.length != data.size()) {
        throw std::runtime_error("Data length does not match the expected value.");
    }
    return data;
}

void GattClient::listen(const std::function<void(std::uint16_t, Buffer)> &callback)
{
    // The events get ofloaded to the queue when reading the read or write request response,
    // because the stream might have contained the events unrelated to the request.
    for (const auto &event : event_queue) {
        callback(std::get<0>(event), std::get<1>(event));
    }
    event_queue.clear();

    client.read([&callback](AttclientAttributeValueEvent<0> metadata, Buffer data) {
        callback(metadata.atthandle, std::move(data));
    });
}

auto GattClient::characteristics() -> Characteristics
{
    Characteristics chr;

    client.write(AttclientFindInformation{connection, 0x0001, 0xFFFF});
    (void)client.read<AttclientFindInformationResponse>();

    bool running = true;
    auto information_found = [&](AttclientFindInformationFoundEvent<0> command, Buffer uuid)
    {
        if (command.length != uuid.size()) {
            throw std::runtime_error("UUID size does not match the expected value.");
        }

        chr[uuid] = command.chrhandle;
    };

    auto procedure_completed = [&running](AttclientProcedureCompletedEvent)
    {
        running = false;
    };

    while(running) {
        client.read(information_found, procedure_completed);
    }

    return chr;
}
