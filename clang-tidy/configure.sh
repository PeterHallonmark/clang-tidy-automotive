#!/usr/bin/bash
mkdir -p build
cd build
cmake -G Ninja \
      -DCMAKE_CXX_COMPILER=/usr/bin/clang++-20 \
      -DCMAKE_C_COMPILER=/usr/bin/clang-20 \
      ..
cd ..
