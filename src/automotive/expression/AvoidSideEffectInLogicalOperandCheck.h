//===--- AvoidSideEffectInLogicalOperandCheck.h - clang-tidy ----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSIDEEFFECTINLOGICALOPERANDCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSIDEEFFECTINLOGICALOPERANDCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-expression
/// @brief Detects persistent side effects in the right operand of && and ||.
///
/// The right-hand operand of a logical && or || operator shall not contain
/// persistent side effects. Due to short-circuit evaluation, the right operand
/// may not be evaluated, making side effects in it unreliable and potentially
/// leading to unexpected behavior.
///
/// @par MISRA C:2025 Rule 13.5
/// The right hand operand of a logical && or || operator shall not contain
/// persistent side effects.
/// @par Category: Required
///
/// Example:
/// @code
///   int x = 0;
///   if (a && (x = foo()))  // Warning: assignment in right operand
///   if (b || bar(++x))     // Warning: increment in function arg
///   if (c && pure_func())  // Compliant: no side effects
/// @endcode
class AvoidSideEffectInLogicalOperandCheck : public ClangTidyCheck {
public:
  AvoidSideEffectInLogicalOperandCheck(StringRef Name,
                                       ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  bool hasSideEffect(const Expr *E) const;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSIDEEFFECTINLOGICALOPERANDCHECK_H
