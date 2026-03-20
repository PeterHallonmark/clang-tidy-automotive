cmake_minimum_required(VERSION 3.13.4)

# Generate clang-tidy config header (required by core)
configure_file(
  clang-tidy-config.h.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/clang-tidy-config.h
)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

add_executable(clang-tidy-standalone
  ClangTidy.cpp
  ClangTidyCheck.cpp
  ClangTidyModule.cpp
  ClangTidyDiagnosticConsumer.cpp
  ClangTidyDiagnosticMapping.cpp
  ClangTidyOptions.cpp
  ClangTidyProfiling.cpp
  ExpandModularHeadersPPCallbacks.cpp
  GlobList.cpp
  NoLintDirectiveHandler.cpp

  tool/ClangTidyMain.cpp
  tool/ClangTidyToolMain.cpp
)

# Make all LLVM/Clang headers available
target_include_directories(clang-tidy-standalone SYSTEM PRIVATE
  ${LLVM_INCLUDE_DIRS}
  ${CLANG_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
)

# Link only the core clang-tidy libraries
target_link_libraries(clang-tidy-standalone PRIVATE

  clangAST
  clangASTMatchers
  clangBasic
  clangTooling
)
