cmake_minimum_required(VERSION 3.13.4)
project(clang-tidy-standalone)

find_package(LLVM 20.1.8 EXACT REQUIRED CONFIG)
find_package(Clang 20.1.8 EXACT REQUIRED CONFIG)

# Generate clang-tidy config header (required by core)
configure_file(
    clang-tidy-config.h.cmake
    ${CMAKE_BINARY_DIR}/clang-tidy-config.h
)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

add_executable(clang-tidy-standalone
    tool/ClangTidyMain.cpp
    tool/ClangTidyToolMain.cpp
)

target_include_directories(clang-tidy-standalone
    PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}
)

# Make all LLVM/Clang headers available
target_include_directories(clang-tidy-standalone
    SYSTEM PRIVATE
        ${LLVM_INCLUDE_DIRS}
        ${CLANG_INCLUDE_DIRS}
)

# Link only the core clang-tidy libraries
target_link_libraries(clang-tidy-standalone
    PRIVATE
        clangTidy
        clangAST
        clangASTMatchers
        clangBasic
        clangTooling
)
