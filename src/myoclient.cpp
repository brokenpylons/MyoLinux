/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "myoclient.h"

#include <utility>

MyoClient::MyoClient(const GattClient &client)
    : client(client)
{ }


void MyoClient::connect()
{
    client.connect(GattClient::Address{0x73, 0x83, 0x1b, 0x61, 0xb3, 0xe2});

    for (const auto &chr : client.characteristics()) {
        const auto uuid = std::get<0>(chr);
        const auto handle = std::get<1>(chr);

        service_handles[static_cast<std::uint16_t>(uuid[13] << 8 | uuid[12])] = handle;
    }
}
