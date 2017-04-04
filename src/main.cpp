/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bled112client.h"
#include "gattclient.h"
#include "myoclient.h"
#include "serial.h"

#include <cinttypes>

using namespace myolinux;

int main()
{
    MyoClient myo(Serial{"/dev/ttyACM0", 115200});
    myo.connect(GattClient::Address{{0x73, 0x83, 0x1b, 0x61, 0xb3, 0xe2}});

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
