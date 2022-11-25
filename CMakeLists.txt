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
project(AwwTools VERSION 1.2.3.4 LANGUAGES CXX)


# Options: Things you can set via commandline options to cmake (e.g. -DENABLE_LTO=[ON|OFF])
option(ENABLE_WARNINGS_SETTINGS "Allow target_set_warnings to add flags and defines.
                                 Set this to OFF if you want to provide your own warning parameters." ON)
option(ENABLE_LTO "Enable link time optimization" ON)
option(ENABLE_DOCTESTS "Include tests in the library. Setting this to OFF will remove all doctest related code.
                        Tests in tests/*.cpp will still be enabled." ON)

# Include stuff. No change needed.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
include(ConfigSafeGuards)
include(Colors)
include(CTest)
include(Doctest)
include(Documentation)
include(LTO)
include(Misc)
include(Warnings)

# Check for LTO support.
find_lto(CXX)

# --------------------------------------------------------------------------------
#                         Locate files (change as needed).
# --------------------------------------------------------------------------------
set(SOURCES          # All .cpp files in src/
    src/example.cpp
    src/aww-common.cpp
    src/os-exec.cpp
)
if(WIN32)
    list(APPEND SOURCES src/aww-common-windows.cpp)
elseif(APPLE)
    list(APPEND SOURCES src/aww-common-apple.cpp)
elseif(UNIX)
    list(APPEND SOURCES src/aww-common-linux.cpp)
else()
    list(APPEND SOURCES src/aww-common-unknown.cpp)
endif()

set(TESTFILES        # All .cpp files in tests/
    tests/aww.cpp
)
set(LIBRARY_NAME awwlib)  # Default name for the library built from src/*.cpp (change if you wish)


# --------------------------------------------------------------------------------
#                            Build! (Change as needed)
# --------------------------------------------------------------------------------


# Compile all sources into a library.
add_library(${LIBRARY_NAME} OBJECT ${SOURCES})

# Lib needs its header files, and users of the library must also see these (PUBLIC). (No change needed)
target_include_directories(${LIBRARY_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third-party/clip-1.5
    ${CMAKE_CURRENT_SOURCE_DIR}/third-party/webview-dbc7864
)
# we add the sub-directories that we want CMake to scan
add_subdirectory(
    ${CMAKE_CURRENT_SOURCE_DIR}/third-party/clip-1.5
    ${CMAKE_CURRENT_BINARY_DIR}/third-party/webview-dbc7864
)

# There's also (probably) doctests within the library, so we need to see this as well.
target_link_libraries(
    ${LIBRARY_NAME} PUBLIC
    doctest
    clip
    webviewlib
)

if(WIN32)
    target_include_directories(${LIBRARY_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/third-party/wintoast-5e441fd03543b999edb663caf8df7be37c0d575c/src
    )

    add_subdirectory(
     ${CMAKE_CURRENT_SOURCE_DIR}/third-party/wintoast-5e441fd03543b999edb663caf8df7be37c0d575c
    )

    target_link_libraries(${LIBRARY_NAME} PUBLIC wintoastlib)
endif()


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

## Aww app
add_executable(aww-app app/aww-app.cpp)   # Name of exec. and location of file.
target_link_libraries(aww-app PRIVATE ${LIBRARY_NAME})  # Link the executable to library (if it uses it).
target_set_warnings(aww-app ENABLE ALL AS_ERROR ALL DISABLE Annoying) # Set warnings (if needed).
target_enable_lto(aww-app optimized)  # enable link-time-optimization if available for non-debug configurations

# Set the properties you require, e.g. what C++ standard to use. Here applied to library and aww (change as needed).
set_target_properties(
    ${LIBRARY_NAME}
        aww
        aww-git-open
        aww-date
        aww-run
        aww-toast
        aww-app
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
