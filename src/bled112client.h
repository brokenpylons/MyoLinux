/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once
#ifndef MYOLINUX_BLED112CLIENT_H
#define MYOLINUX_BLED112CLIENT_H

#include "myolinux.h"
#include "serial.h"
#include "firstargument.h"
#include "bleapi.h"

#include <functional>
#include <map>
#include <iostream>

namespace MYOLINUX_NAMESPACE {

/// Contains the BlueGiga client and auxiliary classes (which are not included, because there are too many of them).
namespace bled112 {

/// Class for communication using the BlueGiga protocol.
class Client {
public:
    Client(const Serial &socket)
        : socket(socket)
    { }

    template <typename T>
    void write(const T &);

    template <typename T>
    void write(const T &, const Buffer &);

    template <typename T>
    T read();

    template <typename T>
    T read(Buffer &);

    template <typename... Functions>
    void read(const Functions&...);

private:
    template <typename Function>
    using DisableIfFirstArgumentIsPartial = std::enable_if<!Partial<typename FirstArgument<Function>::type>::value>;

    template <typename Function>
    using EnableIfFirstArgumentIsPartial = std::enable_if<Partial<typename FirstArgument<Function>::type>::value>;

    Header readHeader();

    template <typename T>
    void checkHeader(const Header &);

    template <typename T>
    T readPayload(const Header &);

    template <typename T>
    T readPayload(const Header &, Buffer &);

    void dispatch(const Header &);

    template <typename Function, typename... Functions>
    auto dispatch(const Header &, const Function &, const Functions&...)
        ->  typename DisableIfFirstArgumentIsPartial<Function>::type;

    template <typename Function, typename... Functions>
    auto dispatch(const Header &, const Function &, const Functions&...)
        -> typename EnableIfFirstArgumentIsPartial<Function>::type;

    Serial socket;
};

/** Write.
 * \param payload the payload */
template <typename T>
void Client::write(const T &payload)
{
    socket.write(pack(getHeader<T>()));
    socket.write(pack(payload));
}

/** Write.
 * \param payload the payload
 * \param[in] leftover additional payload */
template <typename T>
void Client::write(const T &payload, const Buffer &leftover)
{
    socket.write(pack(getHeader<T>(leftover.size())));
    socket.write(pack(payload));
    socket.write(leftover);
}

inline Header Client::readHeader()
{
    return unpack<Header>(socket.read(sizeof(Header)));
}

template <typename T>
void Client::checkHeader(const Header &header)
{
    if (header.cls != T::cls) {
        throw std::runtime_error("Class index does not match the expected value.");
    }
    if (header.cmd != T::cmd) {
        throw std::runtime_error("Command index does not match the expected value.");
    }
    if (!Partial<T>::value && header.length() != sizeof(T)) {
        throw std::runtime_error("Payload size does not match the expected value.");
    }
}

template <typename T>
T Client::readPayload(const Header &header)
{
    return unpack<T>(socket.read(header.length()));
}

template <typename T>
T Client::readPayload(const Header &header, Buffer &leftover)
{
    const auto payload = unpack<T>(socket.read(sizeof(T)));
    leftover = socket.read(header.length() - sizeof(T));
    return payload;
}

/** Read.
 *  \return the payload */
template <typename T>
T Client::read()
{
    const auto header = readHeader();
    checkHeader<T>(header);
    return readPayload<T>(header);
}

/** Read.
 *  \param[out] leftover additional payload
 *  \return the payload */
template <typename T>
T Client::read(Buffer &leftover)
{
    const auto header = readHeader();
    checkHeader<T>(header);
    return readPayload<T>(header, leftover);
}

inline void Client::dispatch(const Header &)
{ }

template <typename Function, typename... Functions>
auto Client::dispatch(const Header &header, const Function &function, const Functions&... functions)
    -> typename DisableIfFirstArgumentIsPartial<Function>::type
{
    using T = typename FirstArgument<Function>::type;

    if (header.cls == T::cls && header.cmd == T::cmd && header.length() == sizeof(T)) {
        function(readPayload<T>(header));
        return;
    }
    dispatch(header, functions...);
}

template <typename Function, typename... Functions>
auto Client::dispatch(const Header &header, const Function &function, const Functions&... functions)
    -> typename EnableIfFirstArgumentIsPartial<Function>::type
{
    using T = typename FirstArgument<Function>::type;

    if (header.cls == T::cls && header.cmd == T::cmd) {
        Buffer leftover;
        const auto payload = readPayload<T>(header, leftover);

        function(std::move(payload), std::move(leftover));
        return;
    }
    dispatch(header, functions...);
}

/** Read the payload of unknown type.
 *  The dispatch works by iterating over a list of functions, the right one is selected based on the first argument.
 *  In the case that the data type is partial an additional argument is required to pass the leftover data.
 *
 *  Accepted function signatures:
 *  - void(Type)
 *  - void(Type, Buffer)
 *
 *  \param functions callbacks */
template <typename... Functions>
void Client::read(const Functions&... functions)
{
    const auto header = readHeader();
    dispatch(header, functions...);
}

}
}

#endif // MYOLINUX_BLED112CLIENT_H
