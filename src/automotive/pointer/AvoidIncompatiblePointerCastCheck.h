//===--- AvoidIncompatiblePointerCastCheck.h - clang-tidy -------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDINCOMPATIBLEPTRCASTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDINCOMPATIBLEPTRCASTCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-pointer
/// @brief Detects casts between pointers to incompatible object types.
///
/// A cast shall not be performed between a pointer to object type and a
/// pointer to a different object type. Such casts can violate alignment
/// requirements and lead to undefined behavior due to strict aliasing rules.
///
/// @par MISRA C:2025 Rule 11.3
/// A cast shall not be performed between a pointer to object type and a
/// pointer to a different object type.
/// @par Category: Required
///
/// Example:
/// @code
///   int *pi = &i;
///   float *pf = (float *)pi;    // Warning: incompatible pointer cast
///   char *pc = (char *)pi;      // Allowed: char* is special case
///   void *pv = (void *)pi;      // Allowed: void* is generic
///   int *pi2 = (int *)pv;       // Allowed: converting back from void*
/// @endcode
class AvoidIncompatiblePointerCastCheck : public ClangTidyCheck {
public:
  AvoidIncompatiblePointerCastCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDINCOMPATIBLEPTRCASTCHECK_H
