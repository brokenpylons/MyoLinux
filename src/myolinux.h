/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once
#ifndef MYOLINUX_H
#define MYOLINUX_H

#ifndef MYOLINUX_NAMESPACE
#define MYOLINUX_NAMESPACE myolinux
#endif

/** \mainpage
 *  This is the documentation for a C++ library for interfacing with the Myo armband on Linux.
 *  This implementation uses the bled112 dongle that is included with the device. The dongle operates over a serial
 *  port, here abstracted as a \link myolinux::Serial Serial \endlink class. Three protocols are needed for the
 *  comminication with the device, each of is implemented in a separate namespace myolinux::bled112, myolinux::gatt and
 *  myolinux::myo.
 *
 *  For general use only constructs defined in the \ref myolinux are needed.
 */

/** \defgroup myolinux Myo for Linux
 *  Constructs needed for communication with the Myo device.
 *
 *  All communication with the device is done using the myolinux::myo::Client class, all other
 *  constructs are only needed as arguments or are returned by a member function.
 *
 *  For a simple example look at main.cpp file. */

/** The main namespace.
 *  It contains all symbols defined in this project, which prevents the pollution of the global scope. Generally
 *  it is fine to introduce them into the file scope by a using-directive, as the majority of the library is
 *  contained inside the \ref bled112, \ref gatt and \ref myo sub-namespaces.
 *
 *  It directly contains all general helper constructs: \ref Buffer, \ref Serial.
 *
 *  By default it is named myolinux, however to avoid any namespace clashes it can be renamed by defining the
 *  MYOLINUX_NAMESPACE at compile time. Please note that programs using a different global namespace name are not
 *  compatible, therefore the library has to be distributed with the program in that case. */
namespace MYOLINUX_NAMESPACE {}

#endif // MYOLINUX_MYOLINUX_H
