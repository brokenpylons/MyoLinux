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

class GattClient {
public:
    using Address = std::array<std::uint8_t, 6>; // Address byte sequence is in network order (probably reversed).
    using Characteristics = std::map<Buffer, std::uint16_t>;
    using Event = std::pair<std::uint16_t, Buffer>;

    GattClient(const Bled112Client &);

    void discover(std::function<bool(std::int8_t, Address, Buffer)>);
    Characteristics characteristics();
    void connect(const Address &);
    void connect(const std::string &);
    bool connected();
    void disconnect();

    void writeAttribute(const std::uint16_t, const Buffer &);
    Buffer readAttribute(const std::uint16_t);
    void listen(const std::function<void(std::uint16_t, Buffer)> &);

private:
    template <typename T>
    T readResponse();

    Bled112Client client;
    bool connected_ = false;
    std::uint8_t connection;
    std::vector<Event> event_queue;
};

template <typename T>
T GattClient::readResponse()
{
    T ret;

    bool running = true;
    const auto handle_response = [&running, &ret](T response)
    {
        running = false;
        ret = response;
    };

    const auto value_event = [this](AttclientAttributeValueEvent<0> metadata, Buffer data)
    {
        const auto handle = metadata.atthandle;
        event_queue.emplace_back(Event{handle, std::move(data)});
    };

    while (running) {
        client.read(handle_response, value_event);
    }

    return ret;
}

void print_address(const GattClient::Address &);

}

#endif // MYOLINUX_GATTCLIENT_H
