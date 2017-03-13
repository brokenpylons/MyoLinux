/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "device.h"

#include <utility>

Device::Device(Serial &&socket)
    : socket(std::move(socket))
{ }

void Device::writeAttribute(const uint8_t handle, const Buffer &payload)
{
    //write.write(AttclientWriteCommand})

}

// TODO fix this
Buffer Device::readAttribute(uint8_t connection, const uint8_t handle)
{
    write(AttclientReadByHandle{connection, handle});
    read<AttclientReadByHandleResponse>();

    const auto response = read<AttclientAttributeValueEvent<1>>();
    const size_t size = response.header.length() - sizeof(AttclientAttributeValueEvent<1>) - 1;

    Buffer buf(size);
    for (int i = 0; i < size; i++) {
        buf[i] = static_cast<char>(response.payload.value[i]);
    }

    return buf;
}
