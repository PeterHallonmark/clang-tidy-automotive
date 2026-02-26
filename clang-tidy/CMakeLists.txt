cmake_minimum_required(VERSION 3.13.4)
project(clang-tidy-standalone)

# Generate clang-tidy config header (required by core)
#configure_file(
#    clang-tidy-config.h.cmake
#    ${CMAKE_BINARY_DIR}/clang-tidy-config.h
#)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

add_executable(clang-tidy-standalone
  ClangTidyToolMain.cpp
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
        clangTidyMain
        clangTidy
        clangAST
        clangASTMatchers
        clangBasic
        clangTooling

        clangTidyAndroidModule
        clangTidyAbseilModule
        clangTidyAlteraModule
        clangTidyBoostModule
        clangTidyBugproneModule
        clangTidyCERTModule
        clangTidyConcurrencyModule
        clangTidyCppCoreGuidelinesModule
        clangTidyDarwinModule
        clangTidyFuchsiaModule
        clangTidyGoogleModule
        clangTidyHICPPModule
        clangTidyLinuxKernelModule
        clangTidyLLVMModule
        clangTidyLLVMLibcModule
        clangTidyMiscModule
        clangTidyModernizeModule
        clangTidyObjCModule
        clangTidyOpenMPModule
        clangTidyPerformanceModule
        clangTidyPortabilityModule
        clangTidyReadabilityModule
        clangTidyZirconModule
)
