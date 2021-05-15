#!/bin/bash
cmake -B build -G Xcode -DCMAKE_TOOLCHAIN_FILE=$VCPKG_CMAKE_FILE
