/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "serial.h"
#include "device.h"
#include "bled112.h"

#include <unistd.h>

#include <cinttypes>
#include <iostream>
#include <iomanip>

void print_header(Header header)
{
    std::cout << static_cast<int>(header.type) << std::endl;
    std::cout << static_cast<int>(header.tech) << std::endl;
    std::cout << static_cast<int>(header.length()) << std::endl;
    std::cout << static_cast<int>(header.cls) << std::endl;
    std::cout << static_cast<int>(header.cmd) << std::endl;
}

void print_address(uint8_t *address)
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

int main()
{
    Serial serial("/dev/ttyACM0", 9800);
    Device dev(std::move(serial));

    std::array<uint8_t, 6> address;
    uint8_t connection;

    dev.write(GapDiscover{1});
    dev.read<GapDiscoverResponse>();

    auto resp = dev.read<GapScanResponseEvent<0>>();
    print_header(resp.header);

    std::copy(resp.payload.sender, resp.payload.sender + 6, std::begin(address));
    print_address(resp.payload.sender);
    std::cout << std::endl;

    dev.write(GapEndProcedure{});
    dev.read<GapEndProcedureResponse>();

    GapConnectDirect conn{{0}, 0, 6, 6, 64, 0};
    std::copy(std::begin(address), std::end(address), std::begin(conn.address));
    dev.write(conn);
    auto resp2 = dev.read<GapConnectDirectResponse>();
    connection = resp2.payload.connection_handle;
    std::cout << "H" << (int)connection << std::endl;

    sleep(1);
    auto resp3 = dev.read<ConnectionStatusEvent>();
    std::cout << "Connected" << std::endl;

    // WARNING: Attribute handling functions don't work
    auto value = dev.readAttribute(connection, 0x17);
    for (int i = 0; i < value.size(); i++) {
        std::cout << (int)value[i] << " ";
    }
}
