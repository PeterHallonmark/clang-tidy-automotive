//===--- MissingExternalArraySizeCheck.h - clang-tidy -----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MISSINGEXTERNALARRAYSIZECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MISSINGEXTERNALARRAYSIZECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-arrays
/// @brief Detects extern array declarations without explicit size.
///
/// Extern array declarations without an explicit size make it impossible
/// to perform bounds checking at compile time. The size should be specified
/// to enable static analysis and prevent buffer overflows.
///
/// @par MISRA C:2025 Rule 8.11
/// When an array with external linkage is declared, its size should be
/// explicitly specified.
/// @par Category: Advisory
///
/// Example:
/// @code
///   extern int arr[];    // Warning: no size specified
///   extern int arr[10];  // OK: size specified
/// @endcode
class MissingExternalArraySizeCheck : public ClangTidyCheck {
public:
  MissingExternalArraySizeCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MISSINGEXTERNALARRAYSIZECHECK_H
