#/usr/bin/bash
# Temporary script to format witch clang-format
find src -name '*.cpp' -o -name '*.h' | xargs build/clang-format -i
