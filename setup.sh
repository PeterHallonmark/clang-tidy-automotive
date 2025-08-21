#!/usr/bin/bash

# Create symbolic links from the clang-tidy-automotive source tree into LLVM.
ln -f -s ../../../src/automotive/ llvm-project-llvmorg-20.1.8/clang-tools-extra/clang-tidy/
ln -f -s ../../../../../test/checkers/automotive llvm-project-llvmorg-20.1.8/clang-tools-extra/test/clang-tidy/checkers/automotive
ln -f -s ../../../src/ClangTidyDiagnosticMapping.cpp llvm-project-llvmorg-20.1.8/clang-tools-extra/clang-tidy/ClangTidyDiagnosticMapping.cpp
ln -f -s ../../../src/ClangTidyDiagnosticMapping.h llvm-project-llvmorg-20.1.8/clang-tools-extra/clang-tidy/ClangTidyDiagnosticMapping.h

# Create symlinks in the src directory.
directories=(
    "cert"
    "misc"
)

src_files=(
    "ClangTidy.h"
    "ClangTidyCheck.h"
    "ClangTidyDiagnosticConsumer.h"
    "ClangTidyModule.h"
    "ClangTidyModuleRegistry.h"
    "ClangTidyOptions.h"
    "ClangTidyProfiling.h"
    "ExpandModularHeadersPPCallbacks.h"
    "FileExtensionsSet.h"
    "GlobList.h"
    "NoLintDirectiveHandler.h"
)

for directory in "${directories[@]}"; do
    ln -f -s "../llvm-project-llvmorg-20.1.8/clang-tools-extra/clang-tidy/${directory}" "src/${directory}"
done

for file in "${src_files[@]}"; do
    ln -f -s "../llvm-project-llvmorg-20.1.8/clang-tools-extra/clang-tidy/${file}" "src/${file}"
done

# Copy the prepatched files to activate Automotive support
cp prepatch/ClangTidyForceLinker.h llvm-project-llvmorg-20.1.8/clang-tools-extra/clang-tidy/ClangTidyForceLinker.h
cp prepatch/ClangTidy.cpp llvm-project-llvmorg-20.1.8/clang-tools-extra/clang-tidy/ClangTidy.cpp
cp prepatch/CMakeLists.txt llvm-project-llvmorg-20.1.8/clang-tools-extra/clang-tidy/CMakeLists.txt
cp prepatch/ClangTidyDiagnosticConsumer.h llvm-project-llvmorg-20.1.8/clang-tools-extra/clang-tidy/ClangTidyDiagnosticConsumer.h
cp prepatch/ClangTidyDiagnosticConsumer.cpp llvm-project-llvmorg-20.1.8/clang-tools-extra/clang-tidy/ClangTidyDiagnosticConsumer.cpp
