#ifndef BLED112CLIENT_H
#define BLED112CLIENT_H

#include "bled112.h"
#include "serial.h"

#include <functional>
#include <map>

class Bled112Client {
public:
    Bled112Client(Serial &&socket)
        : socket(socket)
    { }

    template <typename T>
    void write(const T &);

    template <typename T>
    T read();

    template <typename T>
    T read(Buffer &leftover);

    [[noreturn]]
    void listen();

    template <typename T>
    void addEventHandler(const std::function<void(T)> &);

    template <typename T>
    void removeEventHandler();

private:
    template <typename T>
    Header readHeader();

    struct EventBase {
        virtual void call(const Buffer &) const = 0;
        virtual ~EventBase() = default;
    };

    template <typename T>
    struct Event : public EventBase {
        Event(std::function<void(T)> cb)
            : cb(cb)
        { }

        void call(const Buffer &buf) const override
        {
            cb(unpack<T>(buf));
        }

    private:
        std::function<void(T)> cb;
    };

    Serial socket;
    std::map<std::pair<std::uint8_t, std::uint8_t>, EventBase*> dispatch;
};

template <typename T>
void Bled112Client::write(const T &payload)
{
    socket.write(pack<T>(payload));
}

template <typename T>
Header Bled112Client::readHeader()
{
    const auto header = unpack<Header>(socket.read(sizeof(Header)));
    if (header.cls != T::cls) {
        throw std::runtime_error("Class index does not match the expected value.");
    }
    if (header.cmd != T::cmd) {
        throw std::runtime_error("Command index does not match the expected value.");
    }

    return header;
}

template <typename T>
T Bled112Client::read()
{
    const auto header = readHeader<T>();
    const auto payload = unpack<T>(socket.read(header.length()));
    return payload;
}

template <typename T>
T Bled112Client::read(Buffer &leftover)
{
    const auto header = readHeader<T>();
    const auto payload = unpack<T>(socket.read(sizeof(T)));
    leftover = socket.read(header.length() - sizeof(T));

    return payload;
}

[[noreturn]]
inline void Bled112Client::listen()
{
    while (true) {
        const auto header = unpack<Header>(socket.read(sizeof(Header)));
        if (header.type != 1) {
            throw std::runtime_error("Received packet is not an event.");
        }

        const auto buf = socket.read(header.length());
        dispatch[std::make_pair(header.cls, header.cmd)]->call(buf);
    }
}

template <typename T>
void Bled112Client::addEventHandler(const std::function<void(T)> &cb)
{
    dispatch[std::make_pair(T::cls, T::cmd)] = new Event<T>(cb);
}

template <typename T>
void Bled112Client::removeEventHandler()
{
    dispatch.erase(std::make_pair(T::cls, T::cmd));
}

#endif // BLED112CLIENT_H
