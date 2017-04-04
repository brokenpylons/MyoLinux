/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once
#ifndef MYOLINUX_FIRST_ARGUMENT_H
#define MYOLINUX_FIRST_ARGUMENT_H

#include <type_traits>

namespace MYOLINUX_NAMESPACE {

template <typename T>
struct FirstArgument : FirstArgument<decltype(&T::operator())> { };

template <typename R, typename A, typename... Args>
struct FirstArgument<R(A, Args...)> {
    using type = A;
};

template <typename R, typename A, typename... Args>
struct FirstArgument<R(*)(A, Args...)> {
    using type = A;
};

template <typename T, typename R, typename A, typename... Args>
struct FirstArgument<R (T::*)(A, Args...)> {
    using type = A;
};

template <typename T, typename R, typename A, typename... Args>
struct FirstArgument<R (T::*)(A, Args...) const> {
    using type = A;
};

template <typename T, typename R, typename A, typename... Args>
struct FirstArgument<R (T::*)(const A&, Args...)> {
    using type = A;
};

template <typename T, typename R, typename A, typename... Args>
struct FirstArgument<R (T::*)(const A&, Args...) const> {
    using type = A;
};

}

#endif // MYOLINUX_FIRST_ARGUMENT_H
