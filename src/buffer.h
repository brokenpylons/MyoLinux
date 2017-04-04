/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once
#ifndef MYOLINUX_BUFFER_H
#define MYOLINUX_BUFFER_H

#include "myolinux.h"

#include <vector>

namespace MYOLINUX_NAMESPACE {

using Buffer = std::vector<unsigned char>;

template <typename T>
Buffer pack(const T &payload)
{
    const auto ptr = reinterpret_cast<const char *>(&payload);
    return Buffer{ptr, ptr + sizeof(T)};
}

template <typename T>
T unpack(const Buffer &buf)
{
    const auto ptr = reinterpret_cast<const T *>(buf.data());
    return *ptr;
}

}

#endif // MYOLINUX_BUFFER_H
