# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 2.8)
project(MyoLinux)

set(HEADERS
    src/buffer.h
    src/serial.h
    src/bleapi.h
    src/myohw.h
    src/bled112client.h
    src/gattclient.h
    src/firstargument.h)

set(SRC
   src/generator.py
   src/bleapi.xml
   src/serial.cpp
   src/main.cpp
   src/gattclient.cpp
   ${HEADERS})

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --pedantic")

add_executable(${PROJECT_NAME} ${SRC})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
#target_link_libraries(${PROJECT_NAME})
