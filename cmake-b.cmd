cmake -B build -S . -DCMAKE_BUILD_TYPE=RELEASE &^
cmake --build build &^ 
build\Debug\unit_tests.exe &^ 
build\Debug\unit_tests.exe --list-test-cases