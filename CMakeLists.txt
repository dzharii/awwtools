# This file specifies how the project should be built, using CMake.
# If you are unfamiliar with CMake, don't worry about all the details.
# The sections you might want to edit are marked as such, and
# the comments should hopefully make most of it clear.
#
# For many purposes, you may not need to change anything about this file.

cmake_minimum_required(VERSION 3.14)

# Set project name, version and laguages here. (change as needed)
project(AwwTools VERSION 0.0.1 LANGUAGES CXX)

#2024-01-27 DISABLE warning C4996: warning STL4043: ed_array_iterator, and related factory functions are non-Standard extensions and will be removed in the future...
add_definitions(-D_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING)

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
include(LTO)
include(Misc)
include(Warnings)
include(FetchContent)

# Check for LTO support.
find_lto(CXX)

# --------------------------------------------------------------------------------
#                         Locate files (change as needed).
# --------------------------------------------------------------------------------
set(SOURCES          # All .cpp files in src/
    src/aww-common.cpp
    src/aww-inspiration.cpp
    src/internal/aww-date.cpp
    src/internal/aww-create.cpp
    src/internal/aww-git-open.cpp
    src/internal/aww-guid.cpp
    src/internal/aww-open.cpp
    src/internal/aww-run.cpp
    src/internal/aww-tag.cpp
    src/internal/aww-term.cpp
    src/internal/aww-toast.cpp
    src/internal/aww-api.cpp
    src/internal/aww-stuff.cpp
    
    #nf78vfbcr2j new aww tool? add sources here!
    src/internal/aww.cpp
)
if(WIN32)
    set(OS_NAME "windows")
elseif(UNIX)
    set(OS_NAME "linux")
else()
    set(OS_NAME "unknown")
endif()

if(WIN32)
    list(APPEND SOURCES
        src/aww-common-windows.cpp
        src/aww-banner-windows.cpp
    )
elseif(UNIX)
    list(APPEND SOURCES
        src/aww-common-linux.cpp
        src/aww-banner-linux.cpp
    )
else()
    list(APPEND SOURCES src/aww-common-unknown.cpp)
endif()

set(TESTFILES        # All .cpp files in tests/
    tests/aww.cpp
)
set(LIBRARY_NAME libawwtools)  # Default name for the library built from src/*.cpp (change if you wish)

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
    ${PROJECT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/third-party/clip-1.5
    ${CMAKE_CURRENT_SOURCE_DIR}/third-party/yhirose_cpp-httplib_dbc4af8/include
)

# we add the sub-directories that we want CMake to scan
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third-party/clip-1.5)

## AwwLib

FetchContent_Declare(
    awwlib
    GIT_REPOSITORY https://github.com/dzharii/awwlib-cpp.git
    GIT_TAG        dmytro_zharii/2024-10-13-dev
    GIT_SHALLOW TRUE
    CMAKE_ARGS -DAWWLIB_BUILD_TESTS=OFF -DAWWLIB_BUILD_EXAMPLES=OFF
)

FetchContent_MakeAvailable(awwlib)




## nlohmann_json
FetchContent_Declare(
    json 
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)

## fmt
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG        10.2.1
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(fmt)

## spd::log
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG        v1.13.0
  GIT_SHALLOW    TRUE
)
set(SPDLOG_BUILD_SHARED OFF CACHE BOOL "Build shared library" FORCE)
set(SPDLOG_FMT_EXTERNAL ON  CACHE BOOL "Use external fmt library instead of bundled" FORCE)
FetchContent_MakeAvailable(spdlog)

target_link_libraries(
    ${LIBRARY_NAME} PUBLIC
    
    awwlib
    doctest
    clip
    nlohmann_json::nlohmann_json
    fmt::fmt
    spdlog::spdlog
)

if(WIN32)
    target_include_directories(${LIBRARY_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/third-party/wintoast-5e441fd/src
    )

    add_subdirectory(
     ${CMAKE_CURRENT_SOURCE_DIR}/third-party/wintoast-5e441fd
    )

    target_link_libraries(${LIBRARY_NAME} PUBLIC wintoastlib)
endif()


if(EXISTS "${CMAKE_SOURCE_DIR}/.env")
    file(STRINGS "${CMAKE_SOURCE_DIR}/.env" ENV_FILE_CONTENTS)
    foreach(LINE ${ENV_FILE_CONTENTS})
        if(LINE MATCHES "^BUILD_DIR_BASE=(.*)$")
            set(BUILD_DIR_BASE "${CMAKE_MATCH_1}")
        endif()
    endforeach()
endif()

if(NOT BUILD_DIR_BASE)
    set(BUILD_DIR_BASE "build")
endif()

string(TOLOWER "${CMAKE_BUILD_TYPE}" BUILD_TYPE_LOWER)
set(AWW_TOOLS_BUILD_DIR "${BUILD_DIR_BASE}-${OS_NAME}-${BUILD_TYPE_LOWER}")

set(AWW_TOOLS_BIN_DIR "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${AWW_TOOLS_BIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${AWW_TOOLS_BIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${AWW_TOOLS_BIN_DIR})

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

## aww-api
add_executable(aww-api app/aww-api.cpp)   # Name of exec. and location of file.
target_link_libraries(aww-api PRIVATE ${LIBRARY_NAME})  # Link the executable to library (if it uses it).
target_set_warnings(aww-api ENABLE ALL AS_ERROR ALL DISABLE Annoying) # Set warnings (if needed).
target_enable_lto(aww-api optimized)  # enable link-time-optimization if available for non-debug configurations

## aww-stuff
add_executable(aww-stuff app/aww-stuff.cpp)   # Name of exec. and location of file.
target_link_libraries(aww-stuff PRIVATE ${LIBRARY_NAME})  # Link the executable to library (if it uses it).
target_set_warnings(aww-stuff ENABLE ALL AS_ERROR ALL DISABLE Annoying) # Set warnings (if needed).
target_enable_lto(aww-stuff optimized)  # enable link-time-optimization if available for non-debug configurations



#fia2wwtog0r new aww tool? add entry here!

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
    aww-api
    aww-stuff
    
    #9x1eam6faab new aww tool? add entry here!
)

add_custom_target(copy-runtime-files ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/aww-create-aww
    ${CMAKE_BINARY_DIR}/bin/aww-create-aww
    DEPENDS aww-create)


set_target_properties(
        ${PROJECTS}
      PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

# Set up tests (see tests/CMakeLists.txt).
add_subdirectory(tests)
