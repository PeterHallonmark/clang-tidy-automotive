//===--- MissingElseCheck.h - clang-tidy ------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MISSINGELSECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MISSINGELSECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// An if-else-if chain without a terminating else clause may indicate
/// incomplete logic. The final else should handle unexpected conditions
/// or document that all cases are intentionally covered.
///
/// Example:
/// @code
///   if (x == 1) {
///     // handle 1
///   } else if (x == 2) {
///     // handle 2
///   }  // Warning: missing final else
///
///   if (x == 1) {
///     // handle 1
///   } else if (x == 2) {
///     // handle 2
///   } else {
///     // handle other  // OK
///   }
/// @endcode
class MissingElseCheck : public ClangTidyCheck {
public:
  MissingElseCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MISSINGELSECHECK_H
