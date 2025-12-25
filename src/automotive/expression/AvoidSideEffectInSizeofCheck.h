//===--- AvoidSideEffectInSizeofCheck.h - clang-tidy ------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSIDEEFFECTINSIZEOFCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSIDEEFFECTINSIZEOFCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @brief Detects sizeof operands with potential side effects.
///
/// The operand of the sizeof operator shall not contain any expression which
/// has potential side effects. Since sizeof typically evaluates at compile
/// time, any side effects in its operand would not be executed, which could
/// lead to unexpected behavior.
///
/// Example:
/// @code
///   int x = 0;
///   size_t s1 = sizeof(x++);     // Warning: side effect in sizeof
///   size_t s2 = sizeof(func());  // Warning: function call in sizeof
///   size_t s3 = sizeof(int);     // Compliant: type operand
///   size_t s4 = sizeof(x);       // Compliant: no side effect
/// @endcode
class AvoidSideEffectInSizeofCheck : public ClangTidyCheck {
public:
  AvoidSideEffectInSizeofCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSIDEEFFECTINSIZEOFCHECK_H
