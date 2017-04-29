/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bled112client.h"
#include "gattclient.h"
#include "myoclient.h"
#include "serial.h"

// To use the example outside of the source tree replace with:
//#include "myolinux/bled112client.h"
//#include "myolinux/gattclient.h"
//#include "myolinux/myoclient.h"
//#include "myolinux/serial.h"

#include <cinttypes>

using namespace myolinux;

int main()
{
    MyoClient myo(Serial{"/dev/ttyACM0", 115200});

    // Replace with the address of your device
    //myo.connect(GattClient::Address{{0x73, 0x83, 0x1b, 0x61, 0xb3, 0xe2}});
    myo.connect("E2:B3:61:1B:83:73");

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

    // Read EMG and IMU
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

    myo.onImu([](MyoClient::OrientationSample ori, MyoClient::AccelerometerSample acc, MyoClient::GyroscopeSample gyr)
    {
        std::cout << ori.w << ", " << ori.x << ", " << ori.y << ", " <<  ori.z << std::endl;
        std::cout << acc[0] << ", " << acc[1] << ", " << acc[2] << std::endl;
        std::cout << gyr[0] << ", " << gyr[1] << ", " << gyr[2] << std::endl;
    });

    for (int i = 0; i < 100; i++) {
        myo.listen();

        auto name = myo.deviceName();
        std::cout << name << std::endl;

    }

    // Disconnect
    myo.disconnect();
}
