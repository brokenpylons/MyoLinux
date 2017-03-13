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
    using length_t = decltype(Header::length);
    constexpr auto max = std::numeric_limits<length_t>::max();

    length_t reminder = 0;
    if (sizeof(T) > max) {
        reminder = static_cast<length_t>(sizeof(T) - max);

        // WARNING: If the size of the struct is larger than supported it fails silently.
    }

    return Header{reminder, 0, 0, static_cast<length_t>(sizeof(T) - reminder), T::cls, T::cmd};
}

inline std::size_t getLength(const Header &header)
{
    return header.length + header.length1;
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
