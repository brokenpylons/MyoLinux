/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/* Copyright (c) 2015, Thalmic Labs Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the copyright holder(s) nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S) BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#pragma once
#ifndef MYOAPI_P_H
#define MYOAPI_P_H
#define PACKED __attribute__ ((__packed__))

#include <cinttypes>
#include <initializer_list>

namespace MYOLINUX_NAMESPACE {
namespace myo {

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

namespace {
const auto event_descriptors = {
    IMUDataDescriptor,
    EmgData0Descriptor,
    EmgData1Descriptor,
    EmgData2Descriptor,
    EmgData3Descriptor
};
}

/// Header that every command begins with.
struct PACKED CommandHeader {
    uint8_t command;               ///< Command to send. See myohw_command_t.
    uint8_t payload_size;          ///< Number of bytes in payload.
};

/// Command to set EMG and IMU modes.
struct PACKED CommandSetMode {
    enum { cmd = 0x01 };
    CommandHeader header;          ///< command == myohw_command_set_mode. payload_size = 3.
    uint8_t emg_mode;              ///< EMG sensor mode. See myohw_emg_mode_t.
    uint8_t imu_mode;              ///< IMU mode. See myohw_imu_mode_t.
    uint8_t classifier_mode;       ///< Classifier mode. See myohw_classifier_mode_t.
};

/// Vibration command.
struct PACKED CommandVibrate {
    enum { cmd = 0x03 };
    CommandHeader header;          ///< command == myohw_command_vibrate. payload_size == 1.
    uint8_t type;                  ///< See myohw_vibration_type_t.
};

/// Sleep modes.
struct PACKED CommandSetSleepMode {
    enum { cmd = 0x09 };
    CommandHeader header;           ///< command == myohw_command_set_sleep_mode. payload_size == 1.
    uint8_t sleep_mode;             ///< Sleep mode. See myohw_sleep_mode_t.
};

/// Raw EMG data received form the device
struct PACKED EmgData {
    int8_t sample1[8];              ///< 1st sample of EMG data.
    int8_t sample2[8];              ///< 2nd sample of EMG data.
};

/// Motion event data received form the device
struct PACKED ImuData {
    /// Orientation data, represented as a unit quaternion. Values are multiplied by MYOHW_ORIENTATION_SCALE.
    struct PACKED orientation {
        int16_t w, x, y, z;
    } orientation;

    int16_t accelerometer[3];       ///< Accelerometer data. In units of g. Range of + -16.
                                    ///< Values are multiplied by MYOHW_ACCELEROMETER_SCALE.
    int16_t gyroscope[3];           ///< Gyroscope data. In units of deg/s. Range of + -2000.
                                    ///< Values are multiplied by MYOHW_GYROSCOPE_SCALE.
};

}
}

#undef PACKED
#endif // MYOAPI_P_H
