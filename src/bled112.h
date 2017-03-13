/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once
#ifndef BLED112_H
#define BLED112_H

#include "bleapi.h"
#include "buffer.h"

#include <vector>
#include <limits>
#include <numeric>
#include <iostream>

template <typename T>
Header getHeader()
{
    return Header{sizeof(T) >> 8, 0, 0, sizeof(T) & 0xFF, T::cls, T::cmd};
}

template <typename T>
Buffer pack(const T &payload)
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
