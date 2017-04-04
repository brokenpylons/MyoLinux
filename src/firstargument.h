/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once
#ifndef FIRST_ARGUMENT_H
#define FIRST_ARGUMENT_H

#include <type_traits>

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

#endif // FIRST_ARGUMENT_H
