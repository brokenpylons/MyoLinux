/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once
#ifndef DEVICE_H
#define DEVICE_H

#include "serial.h"
#include "buffer.h"
#include "bled112.h"

#include <cinttypes>
#include <stdexcept>

// TODO: Split the read write and the att functionality
class Device
{
public:
    Device(Serial &&socket);

    void connect();
    void disconnect();

    void addEventHandler();
    void removeEventHandler();

    template <typename T>
    void write(const T &);

    template <typename T>
    Command<T> read();

    void writeAttribute(const uint8_t handle, const Buffer &payload);
    Buffer readAttribute(uint8_t connection, const uint8_t handle);

private:
    Serial socket;
    uint8_t connection = 0;
};

template <typename T>
void Device::write(const T &payload)
{
    socket.write(pack<T>(payload));
}

template <typename T>
Command<T> Device::read()
{
    auto header = unpack<Header>(socket.read(sizeof(Header)));
    if (header.cls != T::cls) {
        throw std::runtime_error("Class index does not match the expected value");
    }
    if (header.cmd != T::cmd) {
        throw std::runtime_error("Command index does not match the expected value");
    }

    auto x = socket.read(getLength(header));
    std::cout << "ss" << x.size() << std::endl;
    for (int i = 0; i < x.size(); i++) {
        std::cout << (int)x[i] << " ";
    }
    std::cout << std::endl;
    auto payload = unpack<T>(x);

    return Command<T>{header, payload};
}


#endif // DEVICE_H
