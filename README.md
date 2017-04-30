# MyoLinux

[![License: MPL 2.0](https://img.shields.io/badge/License-MPL%202.0-brightgreen.svg)](https://opensource.org/licenses/MPL-2.0)

C++ library for interfacing with the Myo armband on Linux. 

## Features

* Full BLED112 protocol implementation (based on [jrowberg/bglib](https://github.com/jrowberg/bglib))
* GAP/GATT wrapper
  * discovering devices
  * discovering characteristics
  * reading/writing attributes
  * listening to notifications
* myohw protocol (based on [thalmiclabs/myo-bluetooth](https://github.com/thalmiclabs/myo-bluetooth))
  * reading info
  * issuing commands (partial)
  * listening to IMU and EMG events

## Installation

Currently the only way to install the library is to compile it from source. I will cover two installation methods and provide a minimal example to get you started.

### Compiling from source

First thing you will need is the source code, you can download it by clicking [here](https://github.com/brokenpylons/MyoLinux/archive/master.zip) or by cloning the repository:

```
git clone https://github.com/brokenpylons/MyoLinux.git
```

The only nonstandard dependency of this project is CMake 2.8 (at least) which may already be installed by your distribution, if not install it. If you plan to modify the library you might also need Python 3.6 for auto-generating the BLED112 protocol implementation, however this step is otherwise not needed as the generated code is already included in the repository.

For an out-of-source build first create a directory inside the project source tree and swich to it:

```
mkdir build
cd build
```

Call CMake to generate the makefile and call Make to build the library. Finally install the library to the system (this will probably require the root permissions):

```
cmake ..
make
make install
```

### CMake

TODO

### Example

``` cpp
#include "myolinux/bled112client.h"
#include "myolinux/gattclient.h"
#include "myolinux/myoclient.h"
#include "myolinux/serial.h"

#include <cinttypes>

using namespace myolinux;

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

    myo.onImu([](MyoClient::OrientationSample ori,
                 MyoClient::AccelerometerSample acc, 
                 MyoClient::GyroscopeSample gyr)
    {
        std::cout << ori.w << ", " << ori.x << ", " << ori.y << ", " <<  ori.z << std::endl;
        std::cout << acc[0] << ", " << acc[1] << ", " << acc[2] << std::endl;
        std::cout << gyr[0] << ", " << gyr[1] << ", " << gyr[2] << std::endl;
    });

    for (int i = 0; i < 100; i++) {
        myo.listen();
    }

    // Disconnect
    myo.disconnect();
}

```

## Usage

The library is split into four layers:

* Serial port
* BLED112 protocol
* GAP/GATT protocol
* myohw protocol

TODO

## Contributing

Please open an issue if you find bugs or any other defficiencies. Pull requests are also wellcome. Note that by contributing to this repository you irrevocably license your work under the MPL 2.0.

## Porting

Despite the name, the library might actually also work on Mac and Windows (using Cygwin) without or with minor modifications. If anyone is interested in testing the library on other platforms open an issue and please report your findings.

## License

This repository is placed under the MPL 2.0. See LICENSE for more details.
