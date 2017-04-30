/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once
#ifndef MYOLINUX_MYOCLIENT_H
#define MYOLINUX_MYOCLIENT_H

#include "myolinux.h"
#include "gattclient.h"
#include "serial.h"

#include <cinttypes>
#include <map>
#include <memory>

namespace MYOLINUX_NAMESPACE {

extern "C" {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#include "myohw.h"
#pragma GCC diagnostic pop
}

class MyoClient {
public:
    using EmgSample = std::array<std::int8_t, 8>;

    // The values are provided as received from the device. To obtain the actual values they must be scaled
    // by the appropriate scaling factor defined in "myohw.h".
    struct OrientationSample {
        std::int16_t w, x, y, z;
    };
    using AccelerometerSample = std::array<std::int16_t, 3>;
    using GyroscopeSample = std::array<std::int16_t, 3>;

    MyoClient(const GattClient &);
    MyoClient(const Serial &);

    void discover(std::function<bool(std::int8_t, GattClient::Address, Buffer)>);
    void connect(const GattClient::Address &);
    void connect(const std::string &);
    void connect();
    bool connected();
    GattClient::Address address();

    void disconnect();

    template <typename Type>
    Type read(const std::uint16_t);
    myohw_fw_info_t info();
    myohw_fw_version_t firmwareVersion();

    template <typename CommandType,  typename... Args>
    void command(const std::uint8_t, Args&&...);
    void vibrate(const std::uint8_t);
    void setMode(const std::uint8_t, const std::uint8_t, const std::uint8_t);

    std::string deviceName();

    void onEmg(const std::function<void(EmgSample)> &);
    void onImu(const std::function<void(OrientationSample, AccelerometerSample, GyroscopeSample)> &);
    void listen();

private:
    // It was easier to hardcode the handle values. This list must be updated if the Myo interface changes in the future.
    // The descriptors don't seem to have a unique UUID, they however have a distinct handle which can be determined
    // using the bluetoothctl tool. The descriptors are listed under the appropriate characteristics.
    enum {
        //ControlService
        MyoInfoCharacteristic         = 0x0,
        FirmwareVersionCharacteristic = 0x17,
        CommandCharacteristic         = 0x19,

        //ImuDataService
        IMUDataCharacteristic         = 0x1c,
        IMUDataDescriptor             = 0x1d,
        //MotionEventCharacteristic   = 0x0,

        //ClssifierService
        //ClassifierEventCharacteristic = 0x0,

        //EmgDataService
        EmgData0Characteristic        = 0x2b,
        EmgData1Characteristic        = 0x2e,
        EmgData2Characteristic        = 0x31,
        EmgData3Characteristic        = 0x34,
        EmgData0Descriptor            = 0x2c,
        EmgData1Descriptor            = 0x2f,
        EmgData2Descriptor            = 0x32,
        EmgData3Descriptor            = 0x35,

        //BatteryService
        //BatteryLevelCharacteristic  = 0x0,

        DeviceName                    = 0x3,
    };

    GattClient client;
    std::function<void(EmgSample)> emg_callback;
    std::function<void(OrientationSample, AccelerometerSample, GyroscopeSample)> imu_callback;

    void enable_notifications();
};

template <typename T>
T MyoClient::read(const std::uint16_t handle)
{
    return unpack<T>(client.readAttribute(handle));
}

template <typename CommandType,  typename... Args>
void MyoClient::command(const std::uint8_t command, Args&&... args)
{
    myohw_command_header_t header{command, sizeof...(args)};
    client.writeAttribute(CommandCharacteristic, pack(CommandType{std::move(header), std::forward<Args>(args)...}));
}

}

#endif // MYOLINUX_MYOCLIENT_H
