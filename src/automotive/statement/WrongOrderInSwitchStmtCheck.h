//===--- WrongOrderInSwitchStmtCheck.h - clang-tidy -*- C++ -*-===============//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_WRONGORDERINSWITCHSTMTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_WRONGORDERINSWITCHSTMTCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-statements
/// @brief Detects default case not at the end of switch statement.
///
/// The default case should appear as the first or last clause in a switch
/// statement for consistency and readability. Having default in the middle
/// can make the code harder to understand.
///
/// @par MISRA C:2025 Rule 16.5
/// A default label shall appear as either the first or the last switch label
/// of a switch statement.
/// @par Category: Required
///
/// Example:
/// @code
///   switch (x) {
///     case 1: break;
///     default: break;  // Warning: default in middle
///     case 2: break;
///   }
/// @endcode
class WrongOrderInSwitchStmtCheck : public ClangTidyCheck {
public:
  WrongOrderInSwitchStmtCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  const SwitchStmt *PrevSwitch = nullptr;
  const DefaultStmt *PrevDefault = nullptr;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_WRONGORDERINSWITCHSTMTCHECK_H
