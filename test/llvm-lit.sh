#!/bin/sh
rm ../test-build -rf
../build/bin/llvm-lit "$@"
