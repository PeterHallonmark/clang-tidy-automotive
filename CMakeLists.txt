cmake_minimum_required(VERSION 3.13.4)
project(clang-tidy-automotive)

option(CT_AUTOMOTIVE_PLUGIN "Build Clang-Tidy Automotive plugin" ON)
option(CT_AUTOMOTIVE_STANDALONE "Build Clang-Tidy with Automotive support" OFF)
option(CT_AUTOMOTIVE_ENABLE_DOCS "Enable documentation targets" ON)

find_package(LLVM 20.1.8 EXACT REQUIRED CONFIG)
find_package(Clang 20.1.8 EXACT REQUIRED CONFIG)

include_directories(
  ${LLVM_INCLUDE_DIRS}
  ${CLANG_INCLUDE_DIRS}
)

add_definitions(${LLVM_DEFINITIONS})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# The clang-tidy out of tree emulation provides enough emulation to be able to
# build the plugin as standalone and still make it possible to copy the
# automotive directory into clang-tidy source tree.
include(ClangTidyOutOfTreeEmulation)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message(STATUS "CT_AUTOMOTIVE_PLUGIN=${CT_AUTOMOTIVE_PLUGIN}")
message(STATUS "CT_AUTOMOTIVE_STANDALONE=${CT_AUTOMOTIVE_STANDALONE}")

add_subdirectory(src/automotive)

if (CT_AUTOMOTIVE_STANDALONE)
  add_subdirectory(clang-tidy)
endif()

if (CT_AUTOMOTIVE_ENABLE_DOCS)
  add_subdirectory(docs)
endif()

add_custom_target(docs DEPENDS doxygen)
