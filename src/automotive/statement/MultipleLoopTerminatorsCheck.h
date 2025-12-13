//===--- MultipleLoopTerminatorsCheck.h - clang-tidy ------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MULTIPLELOOPTERMINATORSCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MULTIPLELOOPTERMINATORSCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-statement
/// @brief Detects loops with multiple break or goto terminators.
///
/// There should be no more than one break or goto statement used to terminate
/// any iteration statement. Multiple exit points from loops make control flow
/// harder to understand and verify.
///
/// @par MISRA C:2025 Rule 15.4
/// There should be no more than one break or goto statement used to terminate
/// any iteration statement.
/// @par Category: Advisory
///
/// Example:
/// @code
///   for (int i = 0; i < 10; i++) {
///       if (x) break;      // First terminator
///       if (y) break;      // Warning: second terminator
///   }
///   while (1) {
///       if (a) break;      // OK: single terminator
///   }
/// @endcode
class MultipleLoopTerminatorsCheck : public ClangTidyCheck {
public:
  MultipleLoopTerminatorsCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MULTIPLELOOPTERMINATORSCHECK_H
