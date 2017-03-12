/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BLED112_H
#define BLED112_H

#include "bleapi.h"
#include "buffer.h"

#include <vector>

template <typename T>
Header getHeader()
{
    return Header{0, 0, 0, sizeof(T), T::cls, T::cmd};
}

template <typename T>
Buffer pack(T payload)
{
    Command<T> cmd = {
        getHeader<T>(),
        payload
    };

    auto cmd_ptr = reinterpret_cast<char *>(&cmd);
    return Buffer(cmd_ptr, cmd_ptr + sizeof(cmd));
}

template <typename T>
T unpack(Buffer buf)
{
    auto ptr = reinterpret_cast<T *>(buf.data());
    return *ptr;
}

#endif // BLED112_H
