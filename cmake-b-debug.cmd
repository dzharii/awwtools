@echo off
call read-env-file.cmd

set BUILD_DIR=%BUILD_DIR_BASE%-windows-debug
cmake -B %BUILD_DIR% -S . -DCMAKE_BUILD_TYPE=DEBUG && ^
call cmake --build %BUILD_DIR% -j --config Debug && ^
call .\%BUILD_DIR%\bin\unit_tests.exe && ^
call .\%BUILD_DIR%\bin\unit_tests.exe --list-test-cases
