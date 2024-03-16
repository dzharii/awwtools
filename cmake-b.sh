#!/bin/env bash
cmake -B cmake-build -S . -DCMAKE_BUILD_TYPE=RELEASE &&\
cmake --build cmake-build &&\
cmake-build\Debug\unit_tests.exe &&\
cmake-build\Debug\unit_tests.exe --list-test-cases