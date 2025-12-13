//===--- AvoidPartialArrayInitCheck.h - clang-tidy --------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDPARTIALARRAYINITCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDPARTIALARRAYINITCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-arrays
/// @brief Detects partially initialized arrays.
///
/// Partial array initialization (where only some elements are explicitly
/// initialized) can be error-prone. It may be unclear which elements are
/// initialized and to what values. Either initialize all elements explicitly
/// or use {0} to zero-initialize the entire array.
///
/// @par MISRA C:2025 Rule 9.3
/// Arrays shall not be partially initialized.
/// @par Category: Required
///
/// Example:
/// @code
///   int arr[5] = {1, 2};  // Warning: partial initialization
///   int ok[5] = {1, 2, 3, 4, 5};  // OK: fully initialized
///   int zeros[5] = {0};   // OK: explicit zero-init
/// @endcode
class AvoidPartialArrayInitCheck : public ClangTidyCheck {
public:
  AvoidPartialArrayInitCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDPARTIALARRAYINITCHECK_H
