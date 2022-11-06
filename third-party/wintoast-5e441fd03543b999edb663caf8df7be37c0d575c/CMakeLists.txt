cmake_minimum_required(VERSION 3.1.2)

project(wintoastlib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(WTOASTLIB_SOURCES src/wintoastlib.cpp)

add_library(wintoastlib ${WTOASTLIB_SOURCES})

target_link_libraries(wintoastlib)
