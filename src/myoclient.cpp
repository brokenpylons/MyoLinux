/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "myoclient.h"
#include "bled112client.h"

#include <utility>

#define ARRAY_SIZEOF(array) (sizeof(array) / sizeof(array[0]))

namespace MYOLINUX_NAMESPACE {

namespace notifications {
const Buffer on{0x1, 0x0};
const Buffer off{0x0, 0x0};
}

namespace {
const std::vector<std::uint8_t> myo_uuid = MYO_SERVICE_INFO_UUID;
}

/// Creates an object for communication with a Myo device from a serial socket.
/// This constructor creates the entire client stack internally. This is the prefered constructor for general use.
/// \param socket the serial socket for communication
MyoClient::MyoClient(const Serial& socket)
    : client(GattClient{Bled112Client{socket}})
{ }

/// Creates an object for communication with a Myo device from a GattClient instance.
/// For debugging purposes the GattClient instance that has been passed to this constructor may still be used for
/// issuing raw GATT commands, if the client has been connected beforehand. This limitation exists because a copy of the
/// instance is stored and the call to the GattClient#connect alters the state of the object.
/// \param client the GattClient instance
MyoClient::MyoClient(const GattClient &client)
    : client(client)
{ }

/// Discovers the nearby Myo devices.
/// The functionality is the same as in GattClient::discover, except that the non-Myo devices are filtered out.
/// \param callback
void MyoClient::discover(std::function<bool(std::int8_t, GattClient::Address, Buffer)> callback)
{
    client.disconnectAll();
    client.discover([&callback](std::int8_t rssi, GattClient::Address address, Buffer data)
    {
        print_address(address);
        if (std::equal(std::prev(std::end(data), static_cast<decltype(data)::difference_type>(myo_uuid.size())),
                       std::end(data), std::begin(myo_uuid))) {
            return callback(rssi, std::move(address), std::move(data));
        }
        return true;
    });
}

void MyoClient::enable_notifications()
{
    client.writeAttribute(EmgData0Descriptor, notifications::on);
    client.writeAttribute(EmgData1Descriptor, notifications::on);
    client.writeAttribute(EmgData2Descriptor, notifications::on);
    client.writeAttribute(EmgData3Descriptor, notifications::on);
    client.writeAttribute(IMUDataDescriptor, notifications::on);
}

/// Connect to the device with the specified address.
/// The address is in network order, so it might be in reverse on your arhitecture. To find the address of your device
/// use the MyoClient::discover method or use the bluetoothctl tool.
/// \param address address of the device
void MyoClient::connect(const GattClient::Address &address)
{
    client.connect(address);
    enable_notifications();
}

/// Connect to the device with the specified address in string form.
/// The address is represented as six hexadecimal digis separated with colons. This is also the format used by the
/// bluetoothctl tool.
/// Example: 01:23:E2:D4:4D:66
/// \param str string form of the address
void MyoClient::connect(const std::string &str)
{
    client.connect(str);
    enable_notifications();
}

/// Auto-connect the client to the first device.
/// The client scans for devices and connects to the first one it finds. The address of the device that the client
/// connected to can then be found using the MyoClient::address method.
void MyoClient::connect()
{
    discover([this](std::int8_t, GattClient::Address address, Buffer)
    {
        connect(address);
        return false;
    });
}

/// Checks whether the client is connected.
/// \return is the client connected
bool MyoClient::connected()
{
    return client.connected();
}

/// Returns the address of the connected device.
/// If the client is not connected an exception is thrown.
/// \return the address of the device
GattClient::Address MyoClient::address()
{
    return client.address();
}

/// Disconnect the client
void MyoClient::disconnect()
{
    client.writeAttribute(EmgData0Descriptor, notifications::off);
    client.writeAttribute(EmgData1Descriptor, notifications::off);
    client.writeAttribute(EmgData2Descriptor, notifications::off);
    client.writeAttribute(EmgData3Descriptor, notifications::off);
    client.writeAttribute(IMUDataDescriptor, notifications::off);
    client.disconnect();
}

/// Get the device info.
/// \return device info
myohw_fw_info_t MyoClient::info()
{
    return read<myohw_fw_info_t>(MyoInfoCharacteristic);
}

/// Get the firmware version.
/// \return firmware version
myohw_fw_version_t MyoClient::firmwareVersion()
{
    return read<myohw_fw_version_t>(FirmwareVersionCharacteristic);
}

/// MyoClient::vibrate
/// \param vibration_type
void MyoClient::vibrate(const std::uint8_t vibration_type)
{
    command<myohw_command_vibrate_t>(myohw_command_vibrate, vibration_type);
}

/// MyoClient::setMode
/// \param emg_mode
/// \param imu_mode
/// \param classifier_mode
void MyoClient::setMode(const std::uint8_t emg_mode, const std::uint8_t imu_mode, const std::uint8_t classifier_mode)
{
    command<myohw_command_set_mode_t>(myohw_command_set_mode, emg_mode, imu_mode, classifier_mode);
}

/// MyoClient::setSleepMode
/// \param sleep_mode
void MyoClient::setSleepMode(const std::uint8_t sleep_mode)
{
    command<myohw_command_set_sleep_mode_t>(myohw_command_set_sleep_mode, sleep_mode);
}

/// MyoClient::deviceName
/// \return
std::string MyoClient::deviceName()
{
    auto buf = client.readAttribute(DeviceName);
    return std::string{std::begin(buf), std::end(buf)};
}

/// MyoClient::onEmg
/// \param callback
void MyoClient::onEmg(const std::function<void(EmgSample)> &callback)
{
    emg_callback = callback;
}

/// MyoClient::onImu
/// \param callback
void MyoClient::onImu(const std::function<void(OrientationSample, AccelerometerSample, GyroscopeSample)> &callback)
{
    imu_callback = callback;
}

/// Wait for the value event and call the appropriate callback.
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
