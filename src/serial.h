/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once
#ifndef SERIAL_H
#define SERIAL_H

#include "buffer.h"

#include <string>
#include <vector>

class Serial {
public:
    Serial(const std::string &, const int);

    Buffer read(const std::size_t);
    std::size_t write(const Buffer &);

private:
    int fd;
};

#endif // SERIAL_H
