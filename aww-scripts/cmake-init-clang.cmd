cmake -H. -Bcmake-build-clang -DCMAKE_BUILD_TYPE=DEBUG -G Ninja -DCMAKE_CXX_COMPILER=clang++.exe -DCMAKE_C_COMPILER=clang.exe -DCMAKE_LINKER=lld-link.exe
