/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once
#ifndef MYOCLIENT_H
#define MYOCLIENT_H

#include "gattclient.h"
#include "myohw.h"

#include <cinttypes>
#include <map>

class MyoClient {
public:
    MyoClient(const GattClient &);

    void connect();

    template <typename T>
    T read(const std::uint16_t);

    template <typename T>
    void write(const std::uint16_t, const T &);

    template <int N, typename... Args>
    void writeCommand(const Args&... args);




    //template <typename T>
    //void notify(const std::uint16_t service);

private:
    GattClient client;
    std::map<std::uint16_t, std::uint16_t> service_handles;
};

template <typename T>
T MyoClient::read(const std::uint16_t service)
{
    const auto handle = service_handles[service];
    return unpack<T>(client.readAttribute(handle));
}

template <typename T>
void MyoClient::write(const std::uint16_t service, const T &payload)
{
    const auto handle = service_handles[service];
    client.writeAttribute(handle, pack(payload));
}

template <int N>
struct Command {
};

template <>
struct Command<myohw_command_set_mode> {
    using type = myohw_command_set_mode_t;
};

template <>
struct Command<myohw_command_vibrate> {
    using type = myohw_command_vibrate_t;
};

template <int N, typename... Args>
void MyoClient::writeCommand(const Args&... args)
{
    write(CommandCharacteristic, typename Command<N>::type{myohw_command_header_t{myohw_command_vibrate, sizeof...(args)}, args...});
}

#endif // MYOCLIENT_H
