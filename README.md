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

First thing you will need is the source code, you can download it by clicking [here](https://github.com/brokenpylons/MyoLinux/archive/master.zip) or by cloning the repository.

```
git clone https://github.com/brokenpylons/MyoLinux.git
```

The only dependency of this project is CMake 2.8 (at least) which may already be installed by your distribution, if not install it. If you plan to modify the library you might also need Python 3.6 for auto-generating the BLED112 protocol implementation, however this step is otherwise not needed as the generated code is already included in the repository.

For an out-of-source build first create a directory inside the project source tree and swich to it.

```
mkdir build
cd build
```

Call CMake to generate the Makefile and call make to build the library. Finally install the library to the system (this will probably require the root permissions).

```
cmake ..
make
make install
```

### CMake

TODO

### Example

TODO

## Usage

The library is split into four layers:

* Serial port
* BLED112 protocol
* GAP/GATT protocol
* myohw protocol

TODO

## Contributing

Please open an issue if you find bugs or any other defficiencies. Pull requests are also wellcome. Note that by contributing to this project you irrevocably license your work under the MPL 2.0.

## Porting

Despite the name, the library might actually also work on Mac and Windows (using Cygwin) without or with minor modifications. If anyone is interested in testing the library on other platforms please open an issue and report your findings.

## License

This repository is placed under the MPL 2.0 license. See LICENSE for more details.
