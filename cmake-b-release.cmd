@echo off
call read-env-file.cmd

if "%AWW_WINDOWS_ARCH%"=="" set "AWW_WINDOWS_ARCH=x64"
if "%AWW_CMAKE_GENERATOR_PLATFORM%"=="" set "AWW_CMAKE_GENERATOR_PLATFORM=x64"

set BUILD_DIR=%BUILD_DIR_BASE%-windows-%AWW_WINDOWS_ARCH%-release
cmake -B %BUILD_DIR% -S . -A %AWW_CMAKE_GENERATOR_PLATFORM% -DCMAKE_BUILD_TYPE=RELEASE && ^
call cmake --build %BUILD_DIR% -j --config Release && ^
call .\%BUILD_DIR%\bin\unit_tests.exe && ^
call .\%BUILD_DIR%\bin\unit_tests.exe --list-test-cases
