@echo off
call read-env-file.cmd

set BUILD_DIR=%BUILD_DIR_BASE%-windows-release
cmake -B %BUILD_DIR% -S . -DCMAKE_BUILD_TYPE=RELEASE && ^
call cmake --build %BUILD_DIR% -j --config Release && ^
call .\%BUILD_DIR%\bin\unit_tests.exe && ^
call .\%BUILD_DIR%\bin\unit_tests.exe --list-test-cases