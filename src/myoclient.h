/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/// \file myoclient.h

#pragma once
#ifndef MYOLINUX_MYOCLIENT_H
#define MYOLINUX_MYOCLIENT_H

#include "myolinux.h"
#include "gattclient.h"
#include "serial.h"
#include "myoapi.h"

#include <cinttypes>
#include <map>
#include <memory>

namespace MYOLINUX_NAMESPACE {
namespace myo {

// The values are provided as received from the device. To obtain the actual values they must be scaled
// by the appropriate scaling factor defined in "myohw.h".

/// EmgSample
using EmgSample = std::array<std::int8_t, 8>;

/// OrientationSample
using OrientationSample = std::array<std::int16_t, 4>;

/// AccelerometerSample
using AccelerometerSample = std::array<std::int16_t, 3>;

/// GyroscopeSample
using GyroscopeSample = std::array<std::int16_t, 3>;

using Address = gatt::Address;
using DisconnectedException = gatt::DisconnectedException;

/// The Client class.
/// This class depends on a gatt::Client instance for issuing GAP/GATT commands to the device.
/// \ingroup myolinux
class Client {
public:
    Client(const Serial &);
    Client(const gatt::Client &);

    void discover(std::function<bool(std::int8_t, Address, Buffer)>);
    void connect(const Address &);
    void connect(const std::string &);
    void connect();
    bool connected();
    Address address();
    void disconnect();

    FwInfo info();
    FwVersion firmwareVersion();

    void vibrate(const Vibration);
    void setMode(const EmgMode, const ImuMode, const ClassifierMode);
    void setSleepMode(const SleepMode);

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

    /// \cond private
    template <typename Type>
    Type read(const std::uint16_t);

    template <typename CommandType,  typename... Args>
    void command(const Command, Args&&...);
    /// \endcond

    void enable_notifications();

    gatt::Client client;
    std::function<void(EmgSample)> emg_callback;
    std::function<void(OrientationSample, AccelerometerSample, GyroscopeSample)> imu_callback;
};

template <typename T>
T Client::read(const std::uint16_t handle)
{
    return unpack<T>(client.readAttribute(handle));
}

template <typename CommandType,  typename... Args>
void Client::command(const Command command, Args&&... args)
{
    CommandHeader header{static_cast<std::uint8_t>(command), sizeof...(args)};
    client.writeAttribute(CommandCharacteristic, pack(CommandType{std::move(header), std::forward<Args>(args)...}));
}

}
}

#undef PACKED
#endif // MYOLINUX_Client_H
