#ifndef SERIAL_H
#define SERIAL_H

#include "buffer.h"

#include <string>
#include <vector>

class Serial {
public:
    Serial(const std::string &, const int);

    Buffer read(std::size_t);
    std::size_t write(Buffer);

private:
    int fd;
};

#endif // SERIAL_H
