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
    myo::Client client(Serial{"/dev/ttyACM0", 115200});

    // Autoconnect to the first Myo device
    client.connect();
    if (!client.connected()) {
        return 1;
    }

    // Print device address
    print_address(client.address());

    // Read firmware version
    auto version = client.firmwareVersion();
    std::cout << version.major << "."
        << version.minor << "."
        << version.patch << "."
        << version.hardware_rev << std::endl;

    // Vibrate
    client.vibrate(myo::Vibration::Medium);

    // Read name
    auto name = client.deviceName();
    std::cout << name << std::endl;

    // Set sleep mode (otherwise the device auto disconnects after a while)
    client.setSleepMode(myo::SleepMode::NeverSleep);

    // Read EMG and IMU
    client.setMode(myo::EmgMode::SendEmg, myo::ImuMode::SendData, myo::ClassifierMode::Disabled);

    client.onEmg([](myo::EmgSample sample)
    {
        for (std::size_t i = 0; i < 8; i++) {
            std::cout << static_cast<int>(sample[i]);
            if (i != 7) {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
    });

    client.onImu([](myo::OrientationSample ori, myo::AccelerometerSample acc, myo::GyroscopeSample gyr)
    {
        std::cout << ori[0] << ", " << ori[1] << ", " << ori[2] << ", " <<  ori[3] << std::endl;
        std::cout << acc[0] << ", " << acc[1] << ", " << acc[2] << std::endl;
        std::cout << gyr[0] << ", " << gyr[1] << ", " << gyr[2] << std::endl;
    });

    for (int i = 0; i < 100; i++) {
        client.listen();

        auto name = client.deviceName();
        std::cout << name << std::endl;
    }

    client.disconnect();
}

#elif EXAMPLE == 1
// An exception will eventually be thrown if sleep mode is set to myohw_sleep_mode_normal
// Set sleep mode to myohw_sleep_mode_never_sleep to avoid this

int main()
{
    myo::Client client(Serial{"/dev/ttyACM0", 115200});

    // Autoconnect to the first Myo device
    client.connect();
    if (!client.connected()) {
        return 1;
    }

    // Print device address
    print_address(client.address());

    // Read firmware version
    auto version = client.firmwareVersion();
    std::cout << version.major << "."
        << version.minor << "."
        << version.patch << "."
        << version.hardware_rev << std::endl;

    // Vibrate
    client.vibrate(myo::Vibration::Medium);

    // Read name
    auto name = client.deviceName();
    std::cout << name << std::endl;

    // Set sleep mode
    client.setSleepMode(myo::SleepMode::Normal);

    // Read EMG and IMU
    client.setMode(myo::EmgMode::SendEmg, myo::ImuMode::SendData, myo::ClassifierMode::Disabled);

    client.onEmg([](myo::EmgSample sample)
    {
        for (std::size_t i = 0; i < 8; i++) {
            std::cout << static_cast<int>(sample[i]);
            if (i != 7) {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
    });

    client.onImu([](myo::OrientationSample ori, myo::AccelerometerSample acc, myo::GyroscopeSample gyr)
    {
        std::cout << ori[0] << ", " << ori[1] << ", " << ori[2] << ", " <<  ori[3] << std::endl;
        std::cout << acc[0] << ", " << acc[1] << ", " << acc[2] << std::endl;
        std::cout << gyr[0] << ", " << gyr[1] << ", " << gyr[2] << std::endl;
    });

    try {
        while (true) {
            client.listen();

            auto name = client.deviceName();
            std::cout << name << std::endl;
        }
    }
    catch(myo::DisconnectedException &) {
        std::cout << "Disconnected" << std::endl;
    }
}

#endif
