/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "gattclient.h"

#include <iostream>
#include <iomanip>
#include <functional>

GattClient::GattClient(Bled112Client &&client)
    : client(std::move(client))
{ }

static void print_address(const uint8_t *address)
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

void GattClient::discover()
{
    Address address;

    client.write(GapDiscover{GapDiscoverModeEnum::DiscoverGeneric});
    client.read<GapDiscoverResponse>();
    const auto resp = client.read<GapScanResponseEvent<32>>();
    print_address(resp.sender);
    std::copy(std::begin(resp.sender), std::end(resp.sender), std::begin(address));

    client.write(GapEndProcedure{});
    client.read<GapEndProcedureResponse>();

    print_address(address.data());
}

void GattClient::connect(const Address &address)
{
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

}

void GattClient::writeAttribute(const std::uint16_t handle, const Buffer &payload)
{
    client.write(AttclientAttributeWrite<0>{connection, handle, static_cast<std::uint8_t>(payload.size())}, payload);
    (void)client.read<AttclientAttributeWriteResponse>();
}

auto GattClient::characteristics() -> Characteristics
{
    Characteristics chr;

    client.write(AttclientFindInformation{connection, 0x0001, 0xFFFF});
    (void)client.read<AttclientFindInformationResponse>();

    bool running = true;
    auto information_found = [&](const AttclientFindInformationFoundEvent<0> &command, const Buffer &uuid)
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
