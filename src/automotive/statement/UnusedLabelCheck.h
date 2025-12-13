//===--- UnusedLabelCheck.h - clang-tidy ------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_UNUSEDLABELCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_UNUSEDLABELCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-statements
/// @brief Detects labels that are declared but never referenced by a goto.
///
/// Unused labels indicate dead code that should be removed. They may be
/// remnants of removed goto statements or incomplete implementations.
///
/// @par MISRA C:2025 Rule 2.6
/// A function should not contain unused label declarations.
/// @par Category: Advisory
///
/// Example:
/// @code
///   void func(void) {
///     unused_label:  // Warning: label never used
///       do_something();
///   }
/// @endcode
class UnusedLabelCheck : public ClangTidyCheck {
public:
  UnusedLabelCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_STANDARDCLANGUAGECHECK_H
