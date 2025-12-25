//===--- AvoidPointerIntegerCastCheck.h - clang-tidy ------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDPOINTERINTEGERCASTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDPOINTERINTEGERCASTCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-pointer
/// @brief Detects casts between pointer and integer types.
///
/// A conversion should not be performed between a pointer to object and an
/// integer type. Such conversions are implementation-defined and may not
/// preserve the pointer value correctly on all architectures.
///
/// Example:
/// @code
///   int *p;
///   int i = (int)p;           // Warning: pointer to integer
///   int *q = (int *)i;        // Warning: integer to pointer
///   uintptr_t u = (uintptr_t)p;  // May be allowed with option
/// @endcode
class AvoidPointerIntegerCastCheck : public ClangTidyCheck {
public:
  AvoidPointerIntegerCastCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDPOINTERINTEGERCASTCHECK_H
