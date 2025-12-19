//===--- MissingBreakInCaseStmtCheck.h - clang-tidy -------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MISSINGBREAKINCASESTMTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MISSINGBREAKINCASESTMTCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Unintentional fall-through between switch cases is a common source of bugs.
/// Each case should explicitly end with a break, return, or throw statement.
/// If fall-through is intentional, it should be clearly documented.
///
/// Example:
/// @code
///   switch (x) {
///     case 1:
///       do_something();
///       // Warning: missing break - falls through to case 2
///     case 2:
///       do_other();
///       break;
///   }
/// @endcode
class MissingBreakInCaseStmtCheck : public ClangTidyCheck {
public:
  MissingBreakInCaseStmtCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  void checkCase(const Stmt *Case, CompoundStmt::const_body_iterator It,
                 const CompoundStmt *Body, ASTContext *Context);
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MISSINGBREAKINCASESTMTCHECK_H
