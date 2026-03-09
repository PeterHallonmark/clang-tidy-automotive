cmake_minimum_required(VERSION 3.13.4)
project(clang-tidy-standalone)

set(CLANG_TIDY_VERSION v20 CACHE STRING "clang-tidy version")

add_subdirectory(${CLANG_TIDY_VERSION})

