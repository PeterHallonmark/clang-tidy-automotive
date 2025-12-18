#!/usr/bin/bash
cd build
ninja clang-tidy clang-format FileCheck
cd ..
