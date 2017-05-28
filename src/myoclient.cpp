/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#define ARRAY_SIZEOF(array) (sizeof(array) / sizeof(array[0]))

#include "myoclient.h"
#include "bled112client.h"
#include "myoapi_p.h"

#include <utility>

namespace MYOLINUX_NAMESPACE {
namespace myo {

namespace {
template <typename T>
T read(gatt::Client client, const std::uint16_t handle)
{
    return unpack<T>(client.readAttribute(handle));
}

template <typename CommandType,  typename... Args>
void command(gatt::Client client, Args&&... args)
{
    CommandHeader header{CommandType::cmd, sizeof...(args)};
    client.writeAttribute(CommandCharacteristic, pack(CommandType{std::move(header), std::forward<Args>(args)...}));
}
}

/** Creates an object for communication with a Myo device from a serial socket.
 *  This constructor creates the entire client stack internally. This is the prefered constructor for general use.
 *  \param socket the serial socket for communication */
Client::Client(const Serial& socket)
    : client(gatt::Client{bled112::Client{socket}})
{ }

/** Creates an object for communication with a Myo device from a gatt::Client instance.
 *  For debugging purposes the gatt::Client instance that has been passed to this constructor may still be used for
 *  issuing raw GATT commands, if the client has been connected beforehand. This limitation exists because a copy of the
 *  instance is stored and the call to the gatt::Client#connect alters the state of the object.
 *  \param client the gatt::Client instance */
Client::Client(const gatt::Client &client)
    : client(client)
{ }

/** Discovers the nearby Myo devices.
 *  The functionality is the same as in gatt::Client::discover, except that the non-Myo devices are filtered out.
 *  \param callback */
void Client::discover(std::function<bool(std::int8_t, Address, Buffer)> callback)
{
    client.disconnectAll();
    client.discover([&callback](std::int8_t rssi, Address address, Buffer data)
    {
        if (std::equal(std::prev(std::end(data), static_cast<decltype(data)::difference_type>(MyoUuid.size())),
                       std::end(data), std::begin(MyoUuid))) {
            return callback(rssi, std::move(address), std::move(data));
        }
        return true;
    });
}

void Client::enable_notifications()
{
    for (const auto &descriptor : event_descriptors) {
         client.writeAttribute(descriptor, gatt::notifications::enable);
    }
}

/** \copybrief gatt::Client::connect
 *
 *  Reviving the connection is only possible if no data has been sent i.e. setMode has not yet been called, otherwise
 *  the device will disconnect automatically when the program exits. There will be a short window before the
 *  disconnect in which the connection cannot be establised. To avoid this always call the disconnect method before
 *  exiting the program or add sleep(1) before the connect call.
 *
 * \copydetails gatt::Client::connect
 */
void Client::connect(const Address &address)
{
    client.connect(address);
    enable_notifications();
}

/// \copydoc gatt::Client::connect(const std::string &str)
void Client::connect(const std::string &str)
{
    client.connect(str);
    enable_notifications();
}

/** Auto-connect the client to the first device.
 *  The client scans for devices and connects to the first one it finds. The address of the device that the client
 *  connected to can then be found using the Client::address method. */
void Client::connect()
{
    discover([this](std::int8_t, Address address, Buffer)
    {
        connect(address);
        return false;
    });
}

/// \copydoc gatt::Client::connected
bool Client::connected()
{
    return client.connected();
}

/// \copydoc gatt::Client::address
Address Client::address()
{
    return client.address();
}

/// \copydoc gatt::Client::disconnect
void Client::disconnect()
{
    for (const auto &descriptor : event_descriptors) {
         client.writeAttribute(descriptor, gatt::notifications::disable);
    }
    client.disconnect();
}

/** Get the device info.
 *  \return device info */
FwInfo Client::info()
{
    return read<FwInfo>(client, MyoInfoCharacteristic);
}

/** Get the firmware version.
 *  \return firmware version */
FwVersion Client::firmwareVersion()
{
    return read<FwVersion>(client, FirmwareVersionCharacteristic);
}

/** Vibrate.
 *  \param vibration_type vibration type */
void Client::vibrate(const Vibration vibration_type)
{
    command<CommandVibrate>(client, static_cast<std::uint8_t>(vibration_type));
}

/** Set the EMG and IMU modes.
 *  \param emg_mode EMG mode
 *  \param imu_mode IMU mode
 *  \param classifier_mode classifier mode */
void Client::setMode(const EmgMode emg_mode, const ImuMode imu_mode, const ClassifierMode classifier_mode)
{
    command<CommandSetMode>(client,
                            static_cast<std::uint8_t>(emg_mode),
                            static_cast<std::uint8_t>(imu_mode),
                            static_cast<std::uint8_t>(classifier_mode));
}

/** Set the sleep mode.
 *  \param sleep_mode sleep mode
 *  \sa DisconnectedException */
void Client::setSleepMode(const SleepMode sleep_mode)
{
    command<CommandSetSleepMode>(client, static_cast<std::uint8_t>(sleep_mode));
}

/** Read the device name.
 *  \return device name */
std::string Client::deviceName()
{
    auto buf = client.readAttribute(DeviceName);
    return std::string{std::begin(buf), std::end(buf)};
}

/** Set the callback for the EMG value event.
 *  The events are sent at 200 Hz.
 *  \param callback */
void Client::onEmg(const std::function<void(EmgSample)> &callback)
{
    emg_callback = callback;
}

/** Set the callback for the IMU value event.
 *  The events are sent at 50 Hz.
 *  The values are provided as received from the device. To obtain the actual values they must be scaled by the
 *  appropriate scaling factor, see \ref OrientationSample, \ref AccelerometerSample, \ref GyroscopeSample.
 *  \param callback */
void Client::onImu(const std::function<void(OrientationSample, AccelerometerSample, GyroscopeSample)> &callback)
{
    imu_callback = callback;
}

/** Wait for the value event and call the appropriate callback.
 *  \throws myo::DisconnectedException */
void Client::listen()
{
    client.listen([this](const std::uint16_t handle, const Buffer payload)
    {
        if (emg_callback && (handle == EmgData0Characteristic ||
                             handle == EmgData1Characteristic ||
                             handle == EmgData2Characteristic ||
                             handle == EmgData3Characteristic)) {
            const auto data = unpack<EmgData>(payload);

            EmgSample sample1;
            std::copy(data.sample1, data.sample1 + ARRAY_SIZEOF(data.sample1), std::begin(sample1));
            emg_callback(std::move(sample1));

            EmgSample sample2;
            std::copy(data.sample2, data.sample2 + ARRAY_SIZEOF(data.sample2), std::begin(sample2));
            emg_callback(std::move(sample2));
        }
        else if (imu_callback && handle == IMUDataCharacteristic) {
            const auto data = unpack<ImuData>(payload);

            OrientationSample orientation_sample;
            orientation_sample[0] = data.orientation.w;
            orientation_sample[1] = data.orientation.x;
            orientation_sample[2] = data.orientation.y;
            orientation_sample[3] = data.orientation.z;

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
}
