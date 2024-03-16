#!/bin/env bash
cmake -B cmake-build -S . -DCMAKE_BUILD_TYPE=RELEASE &&\
cmake --build cmake-build &&\
./cmake-build/unit_tests &&\
./cmake-build/unit_tests --list-test-cases