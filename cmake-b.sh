#!/bin/env bash
cmake -B build -S . -DCMAKE_BUILD_TYPE=RELEASE &&\
cmake --build build -j --config Release &&\
./build/unit_tests &&\
./build/unit_tests --list-test-cases