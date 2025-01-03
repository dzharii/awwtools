cmake -B build -S . -DCMAKE_BUILD_TYPE=RELEASE
call cmake --build build -j --config Release
call .\build\Release\unit_tests.exe
call .\build\Release\unit_tests.exe --list-test-cases