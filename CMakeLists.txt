# This file specifies how the project should be built, using CMake.
# If you are unfamiliar with CMake, don't worry about all the details.
# The sections you might want to edit are marked as such, and
# the comments should hopefully make most of it clear.
#
# For many purposes, you may not need to change anything about this file.

cmake_minimum_required(VERSION 3.14)

# Set project name, version and laguages here. (change as needed)
# Version numbers are available by including "exampleConfig.h" in
# the source. See exampleConfig.h.in for some more details.
project(AwwTools VERSION 0.0.1 LANGUAGES CXX)


# Options: Things you can set via commandline options to cmake (e.g. -DENABLE_LTO=[ON|OFF])
option(ENABLE_WARNINGS_SETTINGS "Allow target_set_warnings to add flags and defines.
                                 Set this to OFF if you want to provide your own warning parameters." ON)
option(ENABLE_LTO "Enable link time optimization" ON)
option(ENABLE_DOCTESTS "Include tests in the library. Setting this to OFF will remove all doctest related code.
                        Tests in tests/*.cpp will still be enabled." ON)

# Include stuff. No change needed.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
option(ALLOW_AWW_APP "Allow the aww app to be built" OFF)

include(ConfigSafeGuards)
include(Colors)
include(CTest)
include(Doctest)
include(LTO)
include(Misc)
include(Warnings)

# Check for LTO support.
find_lto(CXX)

# --------------------------------------------------------------------------------
#                         Locate files (change as needed).
# --------------------------------------------------------------------------------
set(SOURCES          # All .cpp files in src/
    src/aww-common.cpp
    src/aww-app-manifest.cpp
    src/aww-inspiration.cpp
    src/aww-inernal-aww-tag.cpp
    src/internal/aww-date.cpp
    src/internal/aww-create.cpp
    src/internal/aww-git-open.cpp
    src/internal/aww-guid.cpp
)
if(WIN32)
    list(APPEND SOURCES src/aww-common-windows.cpp)
elseif(UNIX)
    list(APPEND SOURCES src/aww-common-linux.cpp)
else()
    list(APPEND SOURCES src/aww-common-unknown.cpp)
endif()

set(TESTFILES        # All .cpp files in tests/
    tests/aww.cpp
)
set(LIBRARY_NAME awwlib)  # Default name for the library built from src/*.cpp (change if you wish)

## Third-party
# https://github.com/nlohmann/json
set(JSON_BuildTests OFF CACHE INTERNAL "")

# --------------------------------------------------------------------------------
#                            Build! (Change as needed)
# --------------------------------------------------------------------------------


# Compile all sources into a library.
add_library(${LIBRARY_NAME} OBJECT ${SOURCES})

# Lib needs its header files, and users of the library must also see these (PUBLIC). (No change needed)
target_include_directories(${LIBRARY_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third-party/clip-1.5
    ${CMAKE_CURRENT_SOURCE_DIR}/third-party/nlohmann-json-3.11.2/single_include
    ${CMAKE_CURRENT_SOURCE_DIR}/third-party/spdlog-1.11.0/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third-party/fmt-9.1.0/include
)

if(ALLOW_AWW_APP)
    # mark webview.h as system header to suppress clang warnings
    target_include_directories(${LIBRARY_NAME} SYSTEM PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/third-party/webview-dbc7864/src
    )
endif()


# we add the sub-directories that we want CMake to scan
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third-party/clip-1.5)

if(ALLOW_AWW_APP)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third-party/webview-dbc7864)
endif()

## nlohmann-json
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third-party/nlohmann-json-3.11.2)
## end of nlohmann-json

## fmt
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third-party/fmt-9.1.0)
## end of fmt

# There's also (probably) doctests within the library, so we need to see this as well.
target_link_libraries(
    ${LIBRARY_NAME} PUBLIC
    doctest
    clip
)

if(ALLOW_AWW_APP)
    target_link_libraries(
        ${LIBRARY_NAME} PUBLIC
        webview
    )
endif()

if(WIN32)
    target_include_directories(${LIBRARY_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/third-party/wintoast-5e441fd/src
    )

    add_subdirectory(
     ${CMAKE_CURRENT_SOURCE_DIR}/third-party/wintoast-5e441fd
    )

    target_link_libraries(${LIBRARY_NAME} PUBLIC wintoastlib)
endif()

target_link_libraries(${LIBRARY_NAME} PRIVATE nlohmann_json::nlohmann_json)

# Set the compile options you want (change as needed).
target_set_warnings(${LIBRARY_NAME} ENABLE ALL AS_ERROR ALL DISABLE Annoying)
# target_compile_options(${LIBRARY_NAME} ... )  # For setting manually.

# Add an executable for the file app/aww.cpp.
# If you add more executables, copy these lines accordingly.
add_executable(aww app/aww.cpp)   # Name of exec. and location of file.
target_link_libraries(aww PRIVATE ${LIBRARY_NAME})  # Link the executable to library (if it uses it).
target_set_warnings(aww ENABLE ALL AS_ERROR ALL DISABLE Annoying) # Set warnings (if needed).
target_enable_lto(aww optimized)  # enable link-time-optimization if available for non-debug configurations

# Aww git open
add_executable(aww-git-open app/aww-git-open.cpp)   # Name of exec. and location of file.
target_link_libraries(aww-git-open PRIVATE ${LIBRARY_NAME})  # Link the executable to library (if it uses it).
target_set_warnings(aww-git-open ENABLE ALL AS_ERROR ALL DISABLE Annoying) # Set warnings (if needed).
target_enable_lto(aww-git-open optimized)  # enable link-time-optimization if available for non-debug configurations

## Aww date
add_executable(aww-date app/aww-date.cpp)   # Name of exec. and location of file.
target_link_libraries(aww-date PRIVATE ${LIBRARY_NAME})  # Link the executable to library (if it uses it).
target_set_warnings(aww-date ENABLE ALL AS_ERROR ALL DISABLE Annoying) # Set warnings (if needed).
target_enable_lto(aww-date optimized)  # enable link-time-optimization if available for non-debug configurations

## Aww run
add_executable(aww-run app/aww-run.cpp)   # Name of exec. and location of file.
target_link_libraries(aww-run PRIVATE ${LIBRARY_NAME})  # Link the executable to library (if it uses it).
target_set_warnings(aww-run ENABLE ALL AS_ERROR ALL DISABLE Annoying) # Set warnings (if needed).
target_enable_lto(aww-run optimized)  # enable link-time-optimization if available for non-debug configurations

## Aww toast
add_executable(aww-toast app/aww-toast.cpp)   # Name of exec. and location of file.
target_link_libraries(aww-toast PRIVATE ${LIBRARY_NAME})  # Link the executable to library (if it uses it).
target_set_warnings(aww-toast ENABLE ALL AS_ERROR ALL DISABLE Annoying) # Set warnings (if needed).
target_enable_lto(aww-toast optimized)  # enable link-time-optimization if available for non-debug configurations

## Aww term
add_executable(aww-term app/aww-term.cpp)   # Name of exec. and location of file.
target_link_libraries(aww-term PRIVATE ${LIBRARY_NAME})  # Link the executable to library (if it uses it).
target_set_warnings(aww-term ENABLE ALL AS_ERROR ALL DISABLE Annoying) # Set warnings (if needed).
target_enable_lto(aww-term optimized)  # enable link-time-optimization if available for non-debug configurations

## Aww open
add_executable(aww-open app/aww-open.cpp)   # Name of exec. and location of file.
target_link_libraries(aww-open PRIVATE ${LIBRARY_NAME})  # Link the executable to library (if it uses it).
target_set_warnings(aww-open ENABLE ALL AS_ERROR ALL DISABLE Annoying) # Set warnings (if needed).
target_enable_lto(aww-open optimized)  # enable link-time-optimization if available for non-debug configurations

## aww-create
add_executable(aww-create app/aww-create.cpp)   # Name of exec. and location of file.
target_link_libraries(aww-create PRIVATE ${LIBRARY_NAME})  # Link the executable to library (if it uses it).
target_set_warnings(aww-create ENABLE ALL AS_ERROR ALL DISABLE Annoying) # Set warnings (if needed).
target_enable_lto(aww-create optimized)  # enable link-time-optimization if available for non-debug configurations

## aww-guid
add_executable(aww-guid app/aww-guid.cpp)   # Name of exec. and location of file.
target_link_libraries(aww-guid PRIVATE ${LIBRARY_NAME})  # Link the executable to library (if it uses it).
target_set_warnings(aww-guid ENABLE ALL AS_ERROR ALL DISABLE Annoying) # Set warnings (if needed).
target_enable_lto(aww-guid optimized)  # enable link-time-optimization if available for non-debug configurations

## aww-tag
add_executable(aww-tag app/aww-tag.cpp)   # Name of exec. and location of file.
target_link_libraries(aww-tag PRIVATE ${LIBRARY_NAME})  # Link the executable to library (if it uses it).
target_set_warnings(aww-tag ENABLE ALL AS_ERROR ALL DISABLE Annoying) # Set warnings (if needed).
target_enable_lto(aww-tag optimized)  # enable link-time-optimization if available for non-debug configurations

if(ALLOW_AWW_APP)
    ## Aww app
    add_executable(aww-app app/aww-app.cpp)   # Name of exec. and location of file.
    target_link_libraries(aww-app PRIVATE ${LIBRARY_NAME})  # Link the executable to library (if it uses it).
    target_set_warnings(aww-app ENABLE ALL AS_ERROR ALL DISABLE Annoying) # Set warnings (if needed).
    target_enable_lto(aww-app optimized)  # enable link-time-optimization if available for non-debug configurations
endif()
# Set the properties you require, e.g. what C++ standard to use. Here applied to library and aww (change as needed).

set(PROJECTS
    ${LIBRARY_NAME}
    aww
    aww-git-open
    aww-date
    aww-run
    aww-toast
    aww-term
    aww-open
    aww-create
    aww-guid
    aww-tag
)

if(ALLOW_AWW_APP)
    list(APPEND PROJECTS aww-app)
endif()


add_custom_target(copy-runtime-files ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/aww-create-aww
    ${CMAKE_BINARY_DIR}/bin/aww-create-aww
    DEPENDS ${MY_TARGET})


set_target_properties(
        ${PROJECTS}
      PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin
)

# Set up tests (see tests/CMakeLists.txt).
add_subdirectory(tests)
