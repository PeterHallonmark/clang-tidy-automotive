//===--- UnusedTagCheck.h - clang-tidy --------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_UNUSEDTAGCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_UNUSEDTAGCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-unused-code
/// @brief Detects unused tag declarations (struct, union, enum tags).
///
/// This check identifies tag declarations that are defined but never used
/// in the code. Unused tags indicate dead code that should be removed to
/// improve code clarity and maintainability.
///
/// @par MISRA C:2025 Rule 2.4
/// A tag should be used when it is declared.
/// @par Category: Advisory
///
/// Example:
/// @code
///   struct UnusedStruct { int x; };  // Warning: unused tag
///   enum UnusedEnum { A, B, C };     // Warning: unused tag
/// @endcode
class UnusedTagCheck : public ClangTidyCheck {
public:
  UnusedTagCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_UNUSEDTAGCHECK_H
