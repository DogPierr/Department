cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=D:/programs/vcpkg/scripts/buildsystems/vcpkg.cmake 
cmake --build ./build --config Release
cmake --build ./build --config Debug