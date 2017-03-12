/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BUFFER_H
#define BUFFER_H

#include <vector>

using Buffer = std::vector<char>;

/*class Buffer
{
    using Container = std::vector<signed char>;
public:
    Buffer(const std::size_t);
    Container::pointer data();
    std::size_t size() const;

private:
    Container bytes;
};*/
#endif // BUFFER_H
