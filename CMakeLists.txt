cmake_minimum_required(VERSION 3.7.2)
project(MyoLinux)

set(HEADERS
    src/buffer.h
    src/serial.h
    src/bled112.h)

set(SRC
   src/buffer.cpp
   src/generator.py
   src/bleapi.xml
   src/bleapi.h
   src/serial.cpp
   src/main.cpp
   ${HEADERS})

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

add_executable(${PROJECT_NAME} ${SRC})
#target_link_libraries(${PROJECT_NAME})
