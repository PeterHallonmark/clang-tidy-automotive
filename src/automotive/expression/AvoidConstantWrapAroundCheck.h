//===--- AvoidConstantWrapAroundCheck.h - clang-tidy -----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDCONSTANTWRAPAROUNDCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDCONSTANTWRAPAROUNDCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-expression
/// @brief Detects unsigned integer wrap-around in constant expressions.
///
/// Evaluating constant expressions should not lead to unsigned integer
/// wrap-around. While wrap-around behavior is well-defined for unsigned types
/// in C, it is often a sign of a programming error when it occurs in constant
/// expressions at compile time.
///
/// @par MISRA C:2023 Rule 12.4
/// Evaluation of constant expressions should not lead to unsigned integer
/// wrap-around.
/// @par Category: Advisory
///
/// Example:
/// @code
///   const unsigned int x = 0u - 1u;        // Warning: wrap-around
///   const unsigned int y = UINT_MAX + 1u;  // Warning: wrap-around
///   #define MAX_PLUS_ONE (UINT_MAX + 1)    // Warning: wrap-around
///   const unsigned int z = 5u + 10u;       // Compliant: no wrap-around
/// @endcode
class AvoidConstantWrapAroundCheck : public ClangTidyCheck {
public:
  AvoidConstantWrapAroundCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDCONSTANTWRAPAROUNDCHECK_H
