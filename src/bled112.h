#ifndef BLED112_H
#define BLED112_H

#include "bleapi.h"
#include "buffer.h"

#include <vector>

template <typename T>
Buffer pack(T payload)
{
    Command<T> cmd = {
        getHeader(&payload),
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
