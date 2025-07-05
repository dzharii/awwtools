#!/bin/env bash
source read-env-file.sh
BUILD_DIR=${BUILD_DIR_BASE}-linux-debug
cmake -B $BUILD_DIR -S . -DCMAKE_BUILD_TYPE=DEBUG &&\
cmake --build $BUILD_DIR -j --config Debug &&\
./$BUILD_DIR/bin/unit_tests &&\
./$BUILD_DIR/bin/unit_tests --list-test-cases
