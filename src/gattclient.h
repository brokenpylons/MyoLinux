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
    //void writeAttribute(std::uint8_t handle, Buffer payload);
    //void readAttribute(Buffer uuid);
    Buffer readAttribute(const std::uint8_t);

private:
    Bled112Client client;
    std::uint8_t connection;
};

#endif // GATTCLIENT_H
