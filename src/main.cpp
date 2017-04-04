/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "myolinux.h"

#include <cinttypes>

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

    //Connect to device
//    cl.discover([](std::int8_t, GattClient::Address address, Buffer) {
//        return false;
//    });

    cl.connect(GattClient::Address{{0x73, 0x83, 0x1b, 0x61, 0xb3, 0xe2}});
    MyoClient myo(cl);

    // Read firmware version
    auto version = myo.firmwareVersion();
    std::cout << version.major << "."
        << version.minor << "."
        << version.patch << "."
        << version.hardware_rev << std::endl;

    // Vibrate
    myo.vibrate(myohw_vibration_medium);

    // Read name
    auto name = myo.deviceName();
    std::cout << name << std::endl;

    // Read EMG
    myo.setMode(myohw_emg_mode_send_emg, myohw_imu_mode_send_data, myohw_classifier_mode_disabled);

    myo.onEmg([](MyoClient::EmgSample sample)
    {
        for (int i = 0; i < 8; i++) {
            std::cout << static_cast<int>(sample[i]);
            if (i != 7) {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
    });

    myo.onImu([](MyoClient::OrientationSample, MyoClient::AccelerometerSample acc, MyoClient::GyroscopeSample)
    {
        std::cout << acc[0] << std::endl;
    });

    while (true) {
        myo.listen();
        //myo.setMode(myohw_emg_mode_none, myohw_imu_mode_send_data, myohw_classifier_mode_disabled);

        auto name = myo.deviceName();
        std::cout << name << std::endl;

    }
}
