/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "myolinux.h"
#include "serial.h"

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include <map>
#include <stdexcept>

namespace MYOLINUX_NAMESPACE {

namespace {
std::map<int, speed_t> baudrates = {
    { 9600,   B9600 },
    { 115200, B115200 },
};

[[noreturn]]
void error(const std::string &message)
{
    throw std::runtime_error(message + ": (" + std::to_string(errno) + ") " + strerror(errno));
}
}

/** Create an object for communication over the serial port.
 *  In case of permission denied exception try running the program with sudo, if that fixes the problem you are not the
 *  member of uucp group. Run sudo usermod -a -G uucp <user> to add an user.
 *  \param device name of the device file
 *  \param baudrate baudrate for the connection (usually 115200)
 */
Serial::Serial(const std::string &device, const int baudrate)
{
    fd = open(device.data(), O_RDWR | O_NOCTTY); // sudo usermod -a -G uucp <user>
    if (fd < 0) {
        error("Cannot open file");
    }

    termios settings = {};
    if (tcgetattr(fd, &settings) != 0) {
        error("Cannot get settings");
    }

    cfsetispeed(&settings, baudrates[baudrate]);
    cfsetospeed(&settings, baudrates[baudrate]);

    settings.c_cflag &= ~static_cast<tcflag_t>(PARENB);
    settings.c_cflag &= ~static_cast<tcflag_t>(CSTOPB);
    settings.c_cflag &= ~static_cast<tcflag_t>(CSIZE);
    settings.c_cflag |= CS8;
    settings.c_cflag |= CREAD | CLOCAL;

    settings.c_cc[VMIN] = 0;
    settings.c_cc[VTIME] = 0;

    cfmakeraw(&settings);
    tcflush(fd, TCIFLUSH);

    if (tcsetattr(fd, TCSANOW, &settings) != 0) {
        error("Cannot set settings");
    }

    int iflags = TIOCM_RTS;
    ioctl(fd, TIOCMBIC, &iflags);

    iflags = TIOCM_DTR;
    ioctl(fd, TIOCMBIS, &iflags);
}

/// Write to serial port.
Buffer Serial::read(const std::size_t size)
{
    Buffer buffer(size);
    ::read(fd, buffer.data(), size);
    return buffer;
}

/// Read from serial port.
std::size_t Serial::write(const Buffer &buffer)
{
    auto size = ::write(fd, buffer.data(), buffer.size());
    if (size < 0) {
        error("Write failed");
    }
    return static_cast<std::size_t>(size);
}

}
