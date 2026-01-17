#!/usr/bin/bash

# Create symbolic links from the clang-tidy-automotive source tree into LLVM.
ln -f -s ../../../src/automotive/ llvm-project-llvmorg-20.1.8/clang-tools-extra/clang-tidy/
ln -f -s ../../../../../test/checkers/automotive llvm-project-llvmorg-20.1.8/clang-tools-extra/test/clang-tidy/checkers/automotive

# Copy the prepatched files to activate Automotive support
cp tool/ClangTidyDiagnosticMapping.cpp llvm-project-llvmorg-20.1.8/clang-tools-extra/clang-tidy/ClangTidyDiagnosticMapping.cpp
cp tool/ClangTidyDiagnosticMapping.h llvm-project-llvmorg-20.1.8/clang-tools-extra/clang-tidy/ClangTidyDiagnosticMapping.h
cp prepatch/ClangTidyForceLinker.h llvm-project-llvmorg-20.1.8/clang-tools-extra/clang-tidy/ClangTidyForceLinker.h
cp prepatch/ClangTidy.cpp llvm-project-llvmorg-20.1.8/clang-tools-extra/clang-tidy/ClangTidy.cpp
cp prepatch/ClangTidyOptions.cpp llvm-project-llvmorg-20.1.8/clang-tools-extra/clang-tidy/ClangTidyOptions.cpp
cp prepatch/ClangTidyOptions.h llvm-project-llvmorg-20.1.8/clang-tools-extra/clang-tidy/ClangTidyOptions.h
cp prepatch/CMakeLists.txt llvm-project-llvmorg-20.1.8/clang-tools-extra/clang-tidy/CMakeLists.txt
cp prepatch/ClangTidyDiagnosticConsumer.h llvm-project-llvmorg-20.1.8/clang-tools-extra/clang-tidy/ClangTidyDiagnosticConsumer.h
cp prepatch/ClangTidyDiagnosticConsumer.cpp llvm-project-llvmorg-20.1.8/clang-tools-extra/clang-tidy/ClangTidyDiagnosticConsumer.cpp
