//===--- AvoidUninitializedReadCheck.h - clang-tidy ------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDUNINITIALIZEDREADCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDUNINITIALIZEDREADCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @brief Detects reads from uninitialized local variables.
///
/// Objects with automatic storage duration must be initialized before their
/// value is read. Reading from an uninitialized variable leads to undefined
/// behavior.
///
/// Example:
/// @code
///   void func() {
///       int x;
///       int y = x;  // Warning: reading uninitialized variable 'x'
///   }
///
///   // Compliant:
///   void func() {
///       int x = 0;  // OK: initialized before use
///       int y = x;
///   }
/// @endcode
class AvoidUninitializedReadCheck : public ClangTidyCheck {
public:
  AvoidUninitializedReadCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDUNINITIALIZEDREADCHECK_H
