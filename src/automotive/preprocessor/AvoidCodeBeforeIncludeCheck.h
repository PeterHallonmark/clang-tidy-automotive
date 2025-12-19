//===--- AvoidCodeBeforeIncludeCheck.h - clang-tidy -------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDCODEBEFOREINCLUDECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDCODEBEFOREINCLUDECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @brief Detects #include directives preceded by code other than preprocessor
/// directives or comments.
///
/// Header file inclusions should appear at the beginning of a file, before any
/// code. Only preprocessor directives and comments should precede #include
/// statements to ensure headers are processed in a clean environment.
///
/// Example:
/// @code
///   int x = 1;           // Code before include
///   #include <stdio.h>   // Warning: code precedes #include
/// @endcode
class AvoidCodeBeforeIncludeCheck : public ClangTidyCheck {
public:
  AvoidCodeBeforeIncludeCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDCODEBEFOREINCLUDECHECK_H
