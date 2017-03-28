/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "serial.h"
#include "bled112client.h"
#include "gattclient.h"
#include "bleapi.h"

#include "myohw.h"

#include <unistd.h>
#include <netinet/in.h>

#include <cinttypes>
#include <iostream>
#include <iomanip>

#include <type_traits>

/*void print_header(Header header)
{
    std::cout << static_cast<int>(header.type) << std::endl;
    std::cout << static_cast<int>(header.tech) << std::endl;
    std::cout << static_cast<int>(header.length()) << std::endl;
    std::cout << static_cast<int>(header.cls) << std::endl;
    std::cout << static_cast<int>(header.cmd) << std::endl;
}*/

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
    Bled112Client dev(std::move(serial));
    GattClient cl(std::move(dev));

    //cl.discover();
    cl.connect(GattClient::Address{0x73, 0x83, 0x1b, 0x61, 0xb3, 0xe2});
    auto x = cl.characteristics();

    auto data = cl.readAttribute<sizeof(myohw_fw_version_t)>(0x17);
    auto mm = unpack<myohw_fw_version_t>(data);

    std::cout << mm.major << std::endl;
    std::cout << mm.minor << std::endl;
    std::cout << mm.patch << std::endl;
    std::cout << mm.hardware_rev << std::endl;

    cl.writeAttribute(0x19, Buffer{0x3, 0x1, 0x1});
}
