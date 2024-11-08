cmake -B build -S . -DCMAKE_BUILD_TYPE=RELEASE
call cmake --build build
call .\build\unit_tests.exe
call .\build\unit_tests.exe --list-test-cases