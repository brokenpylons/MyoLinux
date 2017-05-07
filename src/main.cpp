/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "myoclient.h"
#include "serial.h"

// To use the example outside of the source tree replace with:
//#include "myolinux/myoclient.h"
//#include "myolinux/serial.h"

#include <cinttypes>

using namespace myolinux;

#define EXAMPLE 0

#if EXAMPLE == 0
int main()
{
    MyoClient myo(Serial{"/dev/ttyACM0", 115200});

    // Autoconnect to the first Myo device
    myo.connect();
    if (!myo.connected()) {
        return 1;
    }

    // Print device address
    print_address(myo.address());

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

    // Set sleep mode (otherwise the device auto disconnects after a while)
    myo.setSleepMode(myohw_sleep_mode_never_sleep);

    // Read EMG and IMU
    myo.setMode(myohw_emg_mode_send_emg, myohw_imu_mode_send_data, myohw_classifier_mode_disabled);

    myo.onEmg([](MyoClient::EmgSample sample)
    {
        for (std::size_t i = 0; i < 8; i++) {
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

    myo.disconnect();
}

#elif EXAMPLE == 1
// An exception will eventually be thrown if sleep mode is set to myohw_sleep_mode_normal
// Set sleep mode to myohw_sleep_mode_never_sleep to avoid this

int main()
{
    MyoClient myo(Serial{"/dev/ttyACM0", 115200});

    // Autoconnect to the first Myo device
    myo.connect();
    if (!myo.connected()) {
        return 1;
    }

    // Print device address
    print_address(myo.address());

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

    // Set sleep mode
    myo.setSleepMode(myohw_sleep_mode_normal);

    // Read EMG and IMU
    myo.setMode(myohw_emg_mode_send_emg, myohw_imu_mode_send_data, myohw_classifier_mode_disabled);

    myo.onEmg([](MyoClient::EmgSample sample)
    {
        for (std::size_t i = 0; i < 8; i++) {
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

    try {
        while (true) {
            myo.listen();

            auto name = myo.deviceName();
            std::cout << name << std::endl;
        }
    }
    catch(MyoClient::DisconnectedException &) {
        std::cout << "Disconnected" << std::endl;
    }
}

#endif
