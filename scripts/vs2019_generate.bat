cd ..
mkdir build
cd build
del CMakeCache.txt
cmake .. -B . -DCMAKE_TOOLCHAIN_FILE=%VCPKG_CMAKE_FILE% -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 16 2019"
cd ..
cd scripts