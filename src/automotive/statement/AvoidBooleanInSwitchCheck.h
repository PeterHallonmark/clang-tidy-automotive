//===--- AvoidBooleanInSwitchCheck.h - clang-tidy ---------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDBOOLEANINSWITCHCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDBOOLEANINSWITCHCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-statements
/// @brief Detects switch statements with boolean controlling expressions.
///
/// A switch statement on a boolean expression is redundant since a boolean
/// can only have two values (true/false). An if-else statement is clearer
/// and more appropriate for boolean decisions.
///
/// @par MISRA C:2025 Rule 16.7
/// A switch expression shall not have essentially Boolean type.
/// @par Category: Required
///
/// Example:
/// @code
///   bool flag = true;
///   switch (flag) {  // Warning: boolean in switch
///     case true: break;
///     case false: break;
///   }
/// @endcode
class AvoidBooleanInSwitchCheck : public ClangTidyCheck {
public:
  AvoidBooleanInSwitchCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDBOOLEANINSWITCHCHECK_H
