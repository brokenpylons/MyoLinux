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
    void enable_notifications();

    gatt::Client client;
    std::function<void(EmgSample)> emg_callback;
    std::function<void(OrientationSample, AccelerometerSample, GyroscopeSample)> imu_callback;
};

}
}

#undef PACKED
#endif // MYOLINUX_Client_H
