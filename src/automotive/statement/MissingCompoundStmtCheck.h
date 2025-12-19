//===--- MissingCompoundStmtCheck.h - clang-tidy ----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MISSINGCOMPOUNDSTMTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MISSINGCOMPOUNDSTMTCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Control flow statements (if, else, for, while, do, switch) without braces
/// around their bodies are error-prone. Adding or removing statements can
/// inadvertently change the control flow. Always using braces improves
/// readability and reduces bugs during maintenance.
///
/// Example:
/// @code
///   if (condition)
///     x++;  // Warning: missing braces
///
///   if (condition) {
///     x++;  // OK: has braces
///   }
/// @endcode
class MissingCompoundStmtCheck : public ClangTidyCheck {
public:
  MissingCompoundStmtCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MISSINGCOMPOUNDSTMTCHECK_H
