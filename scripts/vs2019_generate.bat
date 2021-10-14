cd ..
mkdir build
cd build
del CMakeCache.txt
cmake .. -B . -DCMAKE_TOOLCHAIN_FILE=C:/Develop/vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 16 2019"
cd ..
cd scripts