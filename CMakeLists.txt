# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.7.2)
project(MyoLinux)

set(HEADERS
    src/buffer.h
    src/serial.h
    src/bled112.h
    src/bleapi.h
    src/myohw.h
    src/device.h)

set(SRC
   src/buffer.cpp
   src/generator.py
   src/bleapi.xml

   src/serial.cpp
   src/device.cpp
   src/main.cpp
   ${HEADERS})

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

add_executable(${PROJECT_NAME} ${SRC})
#target_link_libraries(${PROJECT_NAME})
