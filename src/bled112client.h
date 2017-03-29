#ifndef BLED112CLIENT_H
#define BLED112CLIENT_H

#include "bleapi.h"
#include "serial.h"
#include "firstargument.h"

#include <functional>
#include <map>

class Bled112Client {
public:
    template <typename Function>
    using DisableIfFirstArgumentIsPartial = std::enable_if<!Partial<typename FirstArgument<Function>::type>::value>;

    template <typename Function>
    using EnableIfFirstArgumentIsPartial = std::enable_if<Partial<typename FirstArgument<Function>::type>::value>;

    Bled112Client(const Serial &socket)
        : socket(socket)
    { }

    template <typename T>
    void write(const T &);

    template <typename T>
    void write(const T &, const Buffer &);

    template <typename T>
    T read();

    template <typename T>
    T read(Buffer &leftover);

    template <typename... Functions>
    void read(const Functions&...);

private:
    template <typename T>
    Header readHeader();

    void dispatch(const Header &);

    template <typename Function, typename... Functions>
    auto dispatch(const Header &, const Function &, const Functions&...)
        ->  typename DisableIfFirstArgumentIsPartial<Function>::type;

    template <typename Function, typename... Functions>
    auto dispatch(const Header &, const Function &, const Functions&...)
        -> typename EnableIfFirstArgumentIsPartial<Function>::type;

    Serial socket;
};

template <typename T>
void Bled112Client::write(const T &payload)
{
    socket.write(pack(getHeader<T>()));
    socket.write(pack(payload));
}

template <typename T>
void Bled112Client::write(const T &payload, const Buffer &leftover)
{
    socket.write(pack(getHeader<T>(leftover.size())));
    socket.write(pack(payload));
    socket.write(leftover);
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
    if (!Partial<T>::value && header.length() != sizeof(T)) {
        throw std::runtime_error("Payload size does not match the expected value.");
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

inline void Bled112Client::dispatch(const Header &) { }

template <typename Function, typename... Functions>
auto Bled112Client::dispatch(const Header &header, const Function &function, const Functions&... functions)
    -> typename DisableIfFirstArgumentIsPartial<Function>::type
{
    using arg_type = typename FirstArgument<Function>::type;

    if (arg_type::cls == header.cls && arg_type::cmd == header.cmd &&
            header.length() == sizeof(arg_type)) {
        function(unpack<arg_type>(socket.read(header.length())));
        return;
    }
    dispatch(header, functions...);
}

template <typename Function, typename... Functions>
auto Bled112Client::dispatch(const Header &header, const Function &function, const Functions&... functions)
    -> typename EnableIfFirstArgumentIsPartial<Function>::type
{
    using arg_type = typename FirstArgument<Function>::type;

    if (arg_type::cls == header.cls && arg_type::cmd == header.cmd) {
        const auto payload = unpack<arg_type>(socket.read(sizeof(arg_type)));
        const auto leftover = socket.read(header.length() - sizeof(arg_type));

        function(payload, leftover);
        return;
    }
    dispatch(header, functions...);
}

template <typename... Functions>
void Bled112Client::read(const Functions&... functions)
{
    const auto header = unpack<Header>(socket.read(sizeof(Header)));
    dispatch(header, functions...);
}

#endif // BLED112CLIENT_H
