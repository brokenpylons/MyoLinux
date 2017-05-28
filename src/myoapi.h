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

/// \defgroup myoapi

#pragma once
#ifndef MYOAPI_H
#define MYOAPI_H
#define PACKED __attribute__ ((__packed__))

#include "myolinux.h"

#include <cinttypes>
#include <vector>

namespace MYOLINUX_NAMESPACE {
namespace myo {

/// UUID of the info service.
/// The UUID is used to identify a Myo device when scanning. This string appears at the end
/// of the vendor specific part of the packet.
/// \ingroup myoapi
const std::vector<std::uint8_t> MyoUuid = {
    0x42, 0x48, 0x12, 0x4a,
    0x7f, 0x2c, 0x48, 0x47,
    0xb9, 0xde, 0x04, 0xa9,
    0x01, 0x00, 0x06, 0xd5
};

/// EMG modes.
/// \ingroup myoapi
enum class EmgMode {
    None       = 0x00, ///< Do not send EMG data.
    SendEmg    = 0x02, ///< Send filtered EMG data.
    SendEmgRaw = 0x03, ///< Send raw (unfiltered) EMG data.
};

/// IMU modes.
/// \ingroup myoapi
enum class ImuMode {
    None       = 0x00, ///< Do not send IMU data or events.
    SendData   = 0x01, ///< Send IMU data streams (accelerometer, gyroscope, and orientation).
    SendEvents = 0x02, ///< Send motion events detected by the IMU (e.g. taps).
    SendAll    = 0x03, ///< Send both IMU data streams and motion events.
    SendRaw    = 0x04, ///< Send raw IMU data streams.
};

/// Classifier modes.
/// \ingroup myoapi
enum class ClassifierMode  {
    Disabled = 0x00, ///< Disable and reset the internal state of the onboard classifier.
    Enabled  = 0x01, ///< Send classifier events (poses and arm events).
};

/// Sleep modes.
/// \ingroup myoapi
enum class SleepMode {
    Normal     = 0, ///< Normal sleep mode; Myo will sleep after a period of inactivity.
    NeverSleep = 1, ///< Never go to sleep.
};

/// Kinds of vibrations.
/// \ingroup myoapi
enum class Vibration {
    None   = 0x00, ///< Do not vibrate.
    Short  = 0x01, ///< Vibrate for a short amount of time.
    Medium = 0x02, ///< Vibrate for a medium amount of time.
    Long   = 0x03, ///< Vibrate for a long amount of time.
};

/// Various parameters that may affect the behaviour of this Myo armband.
/// The Myo library reads this attribute when a connection is established.
/// \ingroup myoapi
struct PACKED FwInfo {
    uint8_t serial_number[6];        ///< Unique serial number of this Myo.
    uint16_t unlock_pose;            ///< Pose that should be interpreted as the unlock pose.
    uint8_t active_classifier_type;  ///< Whether Myo is currently using a built-in or a custom classifier.
    uint8_t active_classifier_index; ///< Index of the classifier that is currently active.
    uint8_t has_custom_classifier;   ///< Whether Myo contains a valid custom classifier. 1 if it does, otherwise 0.
    uint8_t stream_indicating;       ///< Set if the Myo uses BLE indicates to stream data, for reliable capture.
    uint8_t sku;                     ///< SKU value of the device.
    uint8_t reserved[7];             ///< Reserved for future use; populated with zeros.
};

/// Version information for the Myo firmware.
/// Value layout for the myohw_att_handle_fw_version attribute.
/// Minor version is incremented for changes in this interface.
/// Patch version is incremented for firmware changes that do not introduce changes in this interface.
/// \ingroup myoapi
struct PACKED FwVersion {
    uint16_t major;
    uint16_t minor;
    uint16_t patch;
    uint16_t hardware_rev; ///< Myo hardware revision. See myohw_hardware_rev_t.
};

}
}

#undef PACKED
#endif // MYOAPI_H
