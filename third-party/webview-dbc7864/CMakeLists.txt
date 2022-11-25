cmake_minimum_required(VERSION 3.1.2)

project(webviewlib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(webviewlib webview.cpp)
target_link_libraries(webviewlib)
