//===--- AvoidFlexibleArrayMemberCheck.h - clang-tidy -----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDFLEXIBLEARRAYMEMBERCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDFLEXIBLEARRAYMEMBERCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-arrays
/// @brief Detects flexible array members in structures.
///
/// Flexible array members (arrays with unspecified size at the end of a struct)
/// require special handling for memory allocation and can lead to buffer
/// overflows if not managed correctly. They make code less portable and
/// harder to analyze statically.
///
/// @par MISRA C:2025 Rule 18.7
/// Flexible array members shall not be declared.
/// @par Category: Required
///
/// Example:
/// @code
///   struct Message {
///     int length;
///     char data[];  // Warning: flexible array member
///   };
/// @endcode
class AvoidFlexibleArrayMemberCheck : public ClangTidyCheck {
public:
  AvoidFlexibleArrayMemberCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDFLEXIBLEARRAYMEMBERCHECK_H
