/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once
#ifndef MYOLINUX_GATTCLIENT_H
#define MYOLINUX_GATTCLIENT_H

#include "myolinux.h"
#include "bled112client.h"
#include "buffer.h"

#include <array>
#include <cinttypes>
#include <iomanip>
#include <iostream>
#include <map>

namespace MYOLINUX_NAMESPACE {

/// Contains the GATT client and auxiliary classes.
namespace gatt {

namespace notifications {
const Buffer enable{0x1, 0x0};
const Buffer disable{0x0, 0x0};
}

/** Address of the device.
 *  The address byte sequence  is in network order, so it might be in reverse on your arhitecture. To find the address
 *  of your device use the Client::discover method or use the bluetoothctl tool. */
using Address = std::array<std::uint8_t, 6>;

/// A dictionary mapping characteristics UUIDs to handles.
using Characteristics = std::map<Buffer, std::uint16_t>;

/// Exception thrown when the device disconnects.
class DisconnectedException : public std::exception
{ };

/// Class for communication using the GATT protocol.
class Client {
public:
    Client(const bled112::Client &);

    void discover(std::function<bool(std::int8_t, Address, Buffer)>);
    Characteristics characteristics();
    void connect(const Address &);
    void connect(const std::string &);
    bool connected();
    Address address();

    void disconnect();
    void disconnectAll();

    void writeAttribute(const std::uint16_t, const Buffer &);
    Buffer readAttribute(const std::uint16_t);
    void listen(const std::function<void(std::uint16_t, Buffer)> &);

private:
    using Event = std::pair<std::uint16_t, Buffer>;

    void disconnect(const std::uint8_t);

    template <typename T>
    T readResponse();

    bled112::Client client;
    bool connected_ = false;
    Address address_;
    std::uint8_t connection;
    std::vector<Event> event_queue;
};

}

void print_address(const gatt::Address &);

}

#endif // MYOLINUX_GATTCLIENT_H
