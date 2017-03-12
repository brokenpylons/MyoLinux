/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "serial.h"
#include "bled112.h"

#include <unistd.h>
#include <iostream>
#include <iomanip>

int main() {
    Serial serial("/dev/ttyACM1", 9800);

    GapScanResponseEvent<0> x;
    uint8_t handle;

    {
        Buffer data = pack(GapDiscover{1});
        serial.write(data);

        auto data2 = serial.read(sizeof(Header));
        auto hr = unpack<Header>(data2);

        std::cout << (int)hr.type << std::endl;
        std::cout << (int)hr.length1 << std::endl;
        std::cout << (int)hr.length << std::endl;
        std::cout << (int)hr.cls << std::endl;
        std::cout << (int)hr.cmd << std::endl;

        serial.read(2);
    }
    std::cout << std::endl;

    {
        auto data = serial.read(sizeof(Header));
        auto hr = unpack<Header>(data);

        std::cout << (int)hr.type << std::endl;
        std::cout << (int)hr.length1 << std::endl;
        std::cout << (int)hr.length << std::endl;
        std::cout << (int)hr.cls << std::endl;
        std::cout << (int)hr.cmd << std::endl;

        auto data2 = serial.read(42);
        x = unpack<GapScanResponseEvent<0>>(data2);

        for (auto x : data2) {
            std::cout << (int)x << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    {
        Buffer data = pack(GapEndProcedure{});
        serial.write(data);

        auto data2 = serial.read(sizeof(Header));
        auto hr = unpack<Header>(data2);

        std::cout << (int)hr.type << std::endl;
        std::cout << (int)hr.length1 << std::endl;
        std::cout << (int)hr.length << std::endl;
        std::cout << (int)hr.cls << std::endl;
        std::cout << (int)hr.cmd << std::endl;

        serial.read(2);
    }
    std::cout << std::endl;

    {
        GapConnectDirect con{{0}, 0, 6, 6, 64, 0};
        std::copy(std::begin(x.sender), std::end(x.sender), std::begin(con.address));

        Buffer data = pack(con);
        serial.write(data);

        auto data2 = serial.read(sizeof(Header));
        auto hr = unpack<Header>(data2);

        std::cout << (int)hr.type << std::endl;
        std::cout << (int)hr.length1 << std::endl;
        std::cout << (int)hr.length << std::endl;
        std::cout << (int)hr.cls << std::endl;
        std::cout << (int)hr.cmd << std::endl;

        auto data3 = serial.read(3);
        auto cc = unpack<GapConnectDirectResponse>(data3);
        handle = cc.connection_handle;
        std::cout << "H" << (int)handle << std::endl;

        sleep(1);
    }
    std::cout << std::endl;

    {
         auto data2 = serial.read(sizeof(Header));
         auto hr = unpack<Header>(data2);

         std::cout << (int)hr.type << std::endl;
         std::cout << (int)hr.length1 << std::endl;
         std::cout << (int)hr.length << std::endl;
         std::cout << (int)hr.cls << std::endl;
         std::cout << (int)hr.cmd << std::endl;

         auto data3 = serial.read(16);
         auto stat = unpack<ConnectionStatusEvent>(data3);

         std::cout << (int)stat.connection << std::endl;
         std::cout << (int)stat.flags << std::endl;
         std::cout << "L" << (int)stat.latency << std::endl;
    }
    std::cout << std::endl;

    {

        Buffer data = pack(AttclientReadByHandle{handle, 0x17});
        serial.write(data);

        auto data2 = serial.read(sizeof(Header));
        auto hr = unpack<Header>(data2);

        std::cout << (int)hr.type << std::endl;
        std::cout << (int)hr.length1 << std::endl;
        std::cout << (int)hr.length << std::endl;
        std::cout << (int)hr.cls << std::endl;
        std::cout << (int)hr.cmd << std::endl;

        auto data3 = serial.read(3);
    }
    std::cout << std::endl;

    {
        auto data2 = serial.read(sizeof(Header));
        auto hr = unpack<Header>(data2);

        std::cout << (int)hr.type << std::endl;
        std::cout << (int)hr.length1 << std::endl;
        std::cout << (int)hr.length << std::endl;
        std::cout << (int)hr.cls << std::endl;
        std::cout << (int)hr.cmd << std::endl;

         auto data3 = serial.read(13);
         auto ss = unpack<AttclientAttributeValueEvent<9>>(data3);

         for (int i = 0; i < 9; i++) {
             std::cout << (int)ss.value[i] << " ";
         }
    }

    {



    }
}
