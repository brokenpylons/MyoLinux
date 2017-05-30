/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

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

/** Contains the Myo client and auxiliary classes.
 *  \ingroup myolinux */
namespace myo {

/// \copydoc gatt::Address
using Address = gatt::Address;

/** \copybrief gatt::DisconnectedException
 *
 *  This can happen because of inactivity if the myo::SleepMode is set to Normal (set it to NeverSleep to prevent this)
 *  or because the device is sending value events faster than your program is processing them. In the latter case
 *  decrease the processing latency or put the myo::Client::listen method in a separate thread. */
using DisconnectedException = gatt::DisconnectedException;

/** Class for communication with the Myo device.
 *  This class depends on a gatt::Client instance for issuing GAP/GATT commands to the device. */
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
