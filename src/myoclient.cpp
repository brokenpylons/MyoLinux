/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "myoclient.h"
#include "bled112client.h"

#include <utility>

#define ARRAY_SIZEOF(array) (sizeof(array) / sizeof(array[0]))

namespace MYOLINUX_NAMESPACE {

namespace notify {
const Buffer on{0x1, 0x0};
const Buffer off{0x0, 0x0};
}

MyoClient::MyoClient(const GattClient &client)
    : client(client)
{ }

MyoClient::MyoClient(const Serial& socket)
    : client(GattClient{Bled112Client{socket}})
{ }

void MyoClient::enable_notifications()
{
    client.writeAttribute(EmgData0Descriptor, notify::on);
    client.writeAttribute(EmgData1Descriptor, notify::on);
    client.writeAttribute(EmgData2Descriptor, notify::on);
    client.writeAttribute(EmgData3Descriptor, notify::on);
    client.writeAttribute(IMUDataDescriptor, notify::on);
}

void MyoClient::connect(const GattClient::Address &address)
{
    client.connect(address);
    enable_notifications();
}

void MyoClient::connect(const std::string &str)
{
    client.connect(str);
    enable_notifications();
}

void MyoClient::disconnect()
{
    client.writeAttribute(EmgData0Descriptor, notify::off);
    client.writeAttribute(EmgData1Descriptor, notify::off);
    client.writeAttribute(EmgData2Descriptor, notify::off);
    client.writeAttribute(EmgData3Descriptor, notify::off);
    client.writeAttribute(IMUDataDescriptor, notify::off);
    client.disconnect();
}

myohw_fw_info_t MyoClient::info()
{
    return read<myohw_fw_info_t>(MyoInfoCharacteristic);
}

myohw_fw_version_t MyoClient::firmwareVersion()
{
    return read<myohw_fw_version_t>(FirmwareVersionCharacteristic);
}

void MyoClient::vibrate(const std::uint8_t vibration_type)
{
    command<myohw_command_vibrate_t>(myohw_command_vibrate, vibration_type);
}

void MyoClient::setMode(const std::uint8_t emg_mode, const std::uint8_t imu_mode, const std::uint8_t classifier_mode)
{
    command<myohw_command_set_mode_t>(myohw_command_set_mode, emg_mode, imu_mode, classifier_mode);
}

std::string MyoClient::deviceName()
{
    auto buf = client.readAttribute(DeviceName);
    return std::string{std::begin(buf), std::end(buf)};
}

void MyoClient::onEmg(const std::function<void(EmgSample)> &callback)
{
    emg_callback = callback;
}

void MyoClient::onImu(const std::function<void(OrientationSample, AccelerometerSample, GyroscopeSample)> &callback)
{
    imu_callback = callback;
}

void MyoClient::listen()
{
    client.listen([this](const std::uint16_t handle, const Buffer payload)
    {
        if (emg_callback && (handle == EmgData0Characteristic ||
                             handle == EmgData1Characteristic ||
                             handle == EmgData2Characteristic ||
                             handle == EmgData3Characteristic)) {
            const auto data = unpack<myohw_emg_data_t>(payload);

            EmgSample sample1;
            std::copy(data.sample1, data.sample1 + ARRAY_SIZEOF(data.sample1), std::begin(sample1));
            emg_callback(std::move(sample1));

            EmgSample sample2;
            std::copy(data.sample2, data.sample2 + ARRAY_SIZEOF(data.sample2), std::begin(sample2));
            emg_callback(std::move(sample2));
        }
        else if (imu_callback && handle == IMUDataCharacteristic) {
            const auto data = unpack<myohw_imu_data_t>(payload);

            OrientationSample orientation_sample;
            orientation_sample.w = data.orientation.w;
            orientation_sample.x = data.orientation.x;
            orientation_sample.y = data.orientation.y;
            orientation_sample.z = data.orientation.z;

            AccelerometerSample accelerometer_sample;
            std::copy(data.accelerometer,
                      data.accelerometer + ARRAY_SIZEOF(data.accelerometer),
                      std::begin(accelerometer_sample));

            GyroscopeSample gyroscope_sample;
            std::copy(data.gyroscope,
                      data.gyroscope + ARRAY_SIZEOF(data.gyroscope),
                      std::begin(gyroscope_sample));

            imu_callback(std::move(orientation_sample), std::move(accelerometer_sample), std::move(gyroscope_sample));
        }
    });
}

}
