cmake_minimum_required(VERSION 3.13.4)
project(clang-tidy-automotive)

find_package(LLVM 20.1.8 EXACT REQUIRED CONFIG)
find_package(Clang 20.1.8 EXACT REQUIRED CONFIG)

include_directories(
  ${LLVM_INCLUDE_DIRS}
  ${CLANG_INCLUDE_DIRS}
)

add_definitions(${LLVM_DEFINITIONS})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(ClangTidyOutOfTree)

add_subdirectory(src/automotive)
