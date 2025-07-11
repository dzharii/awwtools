cmake_minimum_required(VERSION 3.14)

# List all files containing tests. (Change as needed)
set(TESTFILES        # All .cpp files in tests/
    aww.cpp
    test-aww-common--aww-string.cpp
    test-aww-common--aww-date.cpp
    test-aww-common--aww-Result.cpp
    test-aww-tag.cpp
    test-aww-date.cpp
    test-aww-create.cpp
    test-aww-git-open.cpp
    test-aww-guid.cpp
    test-aww-open.cpp
    test-aww-api.cpp
)

set(TEST_MAIN unit_tests)   # Default name for test executable (change if you wish).
set(TEST_RUNNER_PARAMS "")  # Any arguemnts to feed the test runner (change as needed).

# --------------------------------------------------------------------------------
#                         Make Tests (no change needed).
# --------------------------------------------------------------------------------
add_executable(${TEST_MAIN} ${TESTFILES})
target_link_libraries(${TEST_MAIN} PRIVATE ${LIBRARY_NAME} doctest)
set_target_properties(${TEST_MAIN} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${AWW_TOOLS_BIN_DIR})
target_set_warnings(${TEST_MAIN} ENABLE ALL AS_ERROR ALL DISABLE Annoying) # Set warnings (if needed).

if(MSVC)
    target_compile_options(${TEST_MAIN} PRIVATE /wd4100)
else()
    target_compile_options(${TEST_MAIN} PRIVATE -Wno-unused-parameter)
endif()

set_target_properties(${TEST_MAIN} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

add_test(
    # Use some per-module/project prefix so that it is easier to run only tests for this module
    NAME ${LIBRARY_NAME}.${TEST_MAIN}
    COMMAND ${TEST_MAIN} ${TEST_RUNNER_PARAMS})

# Adds a 'coverage' target.
include(CodeCoverage)
