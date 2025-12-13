//===--- AvoidStaticInArrayParamCheck.h - clang-tidy ------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSTATICINARRAYPARAMCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSTATICINARRAYPARAMCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-function
/// @brief Detects static keyword in array parameter declarations.
///
/// The declaration of an array parameter shall not contain the static keyword
/// between the [ ]. The C99 static keyword in array parameters specifies a
/// minimum array size but is not widely understood and has subtle semantics
/// that can lead to undefined behavior if violated.
///
/// @par MISRA C:2025 Rule 17.6
/// The declaration of an array parameter shall not contain the static keyword
/// between the [ ].
/// @par Category: Mandatory
///
/// Example:
/// @code
///   void func1(int arr[static 10]);  // Warning: static in array parameter
///   void func2(int arr[10]);         // Compliant: no static keyword
///   void func3(int arr[]);           // Compliant: unsized array
///   void func4(int *arr);            // Compliant: pointer parameter
/// @endcode
class AvoidStaticInArrayParamCheck : public ClangTidyCheck {
public:
  AvoidStaticInArrayParamCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSTATICINARRAYPARAMCHECK_H
