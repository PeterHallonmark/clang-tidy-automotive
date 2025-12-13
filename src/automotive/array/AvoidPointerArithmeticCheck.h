//===--- AvoidPointerArithmeticCheck.h - clang-tidy -------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDPOINTERARITHMETICCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDPOINTERARITHMETICCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-array
/// @brief Detects arithmetic operations on pointer types.
///
/// The +, -, += and -= operators should not be applied to an expression of
/// pointer type. Pointer arithmetic is error-prone and can easily lead to
/// out-of-bounds access. Array indexing (ptr[i]) is preferred as it's more
/// readable and intention is clearer.
///
/// @par MISRA C:2025 Rule 18.4
/// The +, -, += and -= operators should not be applied to an expression of
/// pointer type.
/// @par Category: Advisory
///
/// Example:
/// @code
///   int arr[10];
///   int *p = arr;
///
///   p++;            // Warning: pointer arithmetic
///   p += 2;         // Warning: pointer arithmetic
///   int *q = p + 3; // Warning: pointer arithmetic
///   p--;            // Warning: pointer arithmetic
///
///   // Preferred: use array indexing
///   int x = arr[5]; // Compliant
/// @endcode
class AvoidPointerArithmeticCheck : public ClangTidyCheck {
public:
  AvoidPointerArithmeticCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDPOINTERARITHMETICCHECK_H
