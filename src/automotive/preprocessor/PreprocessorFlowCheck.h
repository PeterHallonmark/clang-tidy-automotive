//===--- PreprocessorFlowCheck.h - clang-tidy -------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_PREPROCESSORFLOWCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_PREPROCESSORFLOWCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-preprocessor
/// @brief Monitors preprocessor flow for MISRA compliance checks.
///
/// This check provides infrastructure for monitoring preprocessor directives
/// and their flow through the compilation process. It can detect issues with
/// include guards, macro definitions, and conditional compilation.
///
/// @par Related MISRA C:2025 Rules
/// Dir 4.10 - Precautions shall be taken in order to prevent the contents
/// of a header file being included more than once.
/// @par Category: Required
class PreprocessorFlowCheck : public ClangTidyCheck {
public:
  PreprocessorFlowCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDUNDEFCHECK_H
