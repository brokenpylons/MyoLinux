# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 2.8)
project(myolinux)

set(HEADERS
    src/buffer.h
    src/serial.h
    src/bleapi.h
    src/myohw.h
    src/bled112client.h
    src/gattclient.h
    src/myoclient.h
    src/firstargument.h
    src/myolinux.h)

set(SRC
   src/generator.py
   src/bleapi.xml
   src/serial.cpp
   src/gattclient.cpp
   src/myoclient.cpp
   ${HEADERS})

set(EXAMPLE_SRC
    src/main.cpp
    ${HEADERS})

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --pedantic")

add_library(${PROJECT_NAME} SHARED ${SRC})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)

add_executable(${PROJECT_NAME}_example ${EXAMPLE_SRC})
set_property(TARGET ${PROJECT_NAME}_example PROPERTY CXX_STANDARD 11)
target_link_libraries(${PROJECT_NAME}_example ${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include/myolinux)

set(CPACK_PACKAGE_NAME "myolinux")
set(CPACK_PACKAGE_VERSION "1.0.0")

include(CPack)
