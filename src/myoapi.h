/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/// \defgroup myoapi

#pragma once
#ifndef MYOAPI_H
#define MYOAPI_H
#define PACKED __attribute__ ((__packed__))

#include "myolinux.h"

#include <cinttypes>

#define MYO_SERVICE_BASE_UUID { \
    0x42, 0x48, 0x12, 0x4a,     \
    0x7f, 0x2c, 0x48, 0x47,     \
    0xb9, 0xde, 0x04, 0xa9,     \
    0x00, 0x00, 0x06, 0xd5      \
}

#define MYO_SERVICE_INFO_UUID { \
    0x42, 0x48, 0x12, 0x4a,     \
    0x7f, 0x2c, 0x48, 0x47,     \
    0xb9, 0xde, 0x04, 0xa9,     \
    0x01, 0x00, 0x06, 0xd5      \
}

namespace MYOLINUX_NAMESPACE {
namespace myo {

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

/// Kinds of commands.
/// \ingroup myoapi
enum class Command {
    SetMode                = 0x01, ///< Set EMG and IMU modes.
    Vibrate                = 0x03, ///< Vibrate.
    DeepSleep              = 0x04, ///< Put Myo into deep sleep.
    Vibrate2               = 0x07, ///< Extended vibrate.
    SetSleepMode           = 0x09, ///< Set sleep mode.
    Unlock                 = 0x0a, ///< Unlock Myo.
    UserAction             = 0x0b, ///< Notify user that an action has been recognized / confirmed.
};

/// Various parameters that may affect the behaviour of this Myo armband.
/// The Myo library reads this attribute when a connection is established.
/// \ingroup myoapi
struct PACKED FwInfo
{
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
struct PACKED FwVersion
{
    uint16_t major;
    uint16_t minor;
    uint16_t patch;
    uint16_t hardware_rev; ///< Myo hardware revision. See myohw_hardware_rev_t.
};

/// Header that every command begins with.
/// \ingroup myoapi
struct PACKED CommandHeader {
    uint8_t command;        ///< Command to send. See myohw_command_t.
    uint8_t payload_size;   ///< Number of bytes in payload.
};

/// Command to set EMG and IMU modes.
/// \ingroup myoapi
struct PACKED CommandSetMode {
    CommandHeader header;          ///< command == myohw_command_set_mode. payload_size = 3.
    uint8_t emg_mode;              ///< EMG sensor mode. See myohw_emg_mode_t.
    uint8_t imu_mode;              ///< IMU mode. See myohw_imu_mode_t.
    uint8_t classifier_mode;       ///< Classifier mode. See myohw_classifier_mode_t.
};

/// Vibration command.
/// \ingroup myoapi
struct PACKED CommandVibrate {
    CommandHeader header;          ///< command == myohw_command_vibrate. payload_size == 1.
    uint8_t type;                  ///< See myohw_vibration_type_t.
};

/// Sleep modes.
/// \ingroup myoapi
struct PACKED CommandSetSleepMode {
    CommandHeader header;          ///< command == myohw_command_set_sleep_mode. payload_size == 1.
    uint8_t sleep_mode;            ///< Sleep mode. See myohw_sleep_mode_t.
};

/// Raw EMG data received in a myohw_att_handle_emg_data_# attribute.
/// Value layout for myohw_att_handle_emg_data_#.
/// \ingroup myoapi
struct PACKED myohw_emg_data_t {
    int8_t sample1[8];       ///< 1st sample of EMG data.
    int8_t sample2[8];       ///< 2nd sample of EMG data.
};

/// Motion event data received in a myohw_att_handle_motion_event attribute.
/// \ingroup myoapi
struct PACKED myohw_imu_data_t {
    /// Orientation data, represented as a unit quaternion. Values are multiplied by MYOHW_ORIENTATION_SCALE.
    struct PACKED orientation {
        int16_t w, x, y, z;
    } orientation;

    int16_t accelerometer[3]; ///< Accelerometer data. In units of g. Range of + -16.
                              ///< Values are multiplied by MYOHW_ACCELEROMETER_SCALE.
    int16_t gyroscope[3];     ///< Gyroscope data. In units of deg/s. Range of + -2000.
                              ///< Values are multiplied by MYOHW_GYROSCOPE_SCALE.
};

}
}

#undef PACKED
#endif // MYOAPI_H
