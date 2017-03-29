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
    Serial serial("/dev/ttyACM0", 115200);
    Bled112Client dev(serial);
    GattClient cl(dev);

    // Connect to device
    cl.connect(GattClient::Address{{0x73, 0x83, 0x1b, 0x61, 0xb3, 0xe2}});

    // Read firmware version
    auto version = unpack<myohw_fw_version_t>(cl.readAttribute(0x17));
    std::cout << version.major << "."
        << version.minor << "."
        << version.patch << "."
        << version.hardware_rev << std::endl;

    // Vibrate
    cl.writeAttribute(0x19, Buffer{0x03, 0x01, 0x02});

    // Read name
    auto name = cl.readAttribute(0x3);
    for (auto x : name) {
        std::cout << x;
    }
    std::cout << std::endl;

    // Read EMG
    cl.writeAttribute(0x19, Buffer{0x1, 0x3, 0x2, 0x0, 0x0});
    cl.writeAttribute(0x2c, Buffer{0x1, 0x0});
    cl.writeAttribute(0x2f, Buffer{0x1, 0x0});
    cl.writeAttribute(0x32, Buffer{0x1, 0x0});
    cl.writeAttribute(0x35, Buffer{0x1, 0x0});

    // Read EMG data (Legacy mode)
    //cl.writeAttribute(0x19, Buffer{0x1, 0x3, 0x1, 0x0, 0x0});
    //cl.writeAttribute(0x28, Buffer{0x1, 0x0});

    auto cb = [&](const std::uint16_t, const Buffer &data)
    {
        auto values = unpack<myohw_emg_data_t>(data);
        for (int i = 0; i < 8; i++) {
            std::cout << static_cast<int>(values.sample1[i]);
            if (i != 7) {
                std::cout << ", ";
            }
        }
        for (int i = 0; i < 8; i++) {
            std::cout << static_cast<int>(values.sample2[i]);
            if (i != 7) {
                std::cout << ", ";
            }
        }

        std::cout << std::endl;
    };

    while (true) {
        cl.readAttribute(cb);
    }
}
