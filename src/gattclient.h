/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once
#ifndef GATTCLIENT_H
#define GATTCLIENT_H

#include "buffer.h"
#include "bled112client.h"

#include <cstdint>

#include <array>

class GattClient {
public:
    using Address = std::array<std::uint8_t, 6>;

    GattClient(Bled112Client &&);

    void discover();
    void connect(const Address &);
    void disconnect();

    //void writeAttribute(Buffer uuid, Buffer payload);
    template <int N>
    void writeAttribute(const std::uint8_t, const Buffer &);
    //void readAttribute(Buffer uuid);

    template <int N>
    Buffer readAttribute(const std::uint8_t);

private:
    Bled112Client client;
    std::uint8_t connection;
};

template <int N>
void GattClient::writeAttribute(const std::uint8_t handle, const Buffer &payload)
{
    AttclientAttributeWrite<N> command{connection, handle, {}};
    std::copy(std::begin(payload), std::end(payload), std::begin(command.data));
    client.write(command);

    (void)client.read<AttclientAttributeWriteResponse>();
}

template <int N>
Buffer GattClient::readAttribute(const std::uint8_t handle)
{
    client.write(AttclientReadByHandle{connection, handle});
    client.read<AttclientReadByHandleResponse>();

    const auto response = client.read<AttclientAttributeValueEvent<N>>();
    // TODO: Check if everything works out...
    return Buffer{std::begin(response.value), std::end(response.value)};
}

#endif // GATTCLIENT_H
