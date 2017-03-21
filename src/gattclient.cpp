/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "gattclient.h"

#include <iostream>
#include <iomanip>

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
    const auto resp = client.read<GapScanResponseEvent<0>>();
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

    client.read<ConnectionStatusEvent>();
    std::cout << "Connected" << std::endl;
}

void GattClient::disconnect()
{

}
