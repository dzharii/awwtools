#!/bin/env bash
#!/bin/env bash
source read-env-file.sh
BUILD_DIR=${BUILD_DIR_BASE}-linux-release
cmake -B $BUILD_DIR -S . -DCMAKE_BUILD_TYPE=RELEASE &&\
cmake --build $BUILD_DIR -j --config Release &&\
./$BUILD_DIR/bin/unit_tests &&\
./$BUILD_DIR/bin/unit_tests --list-test-cases