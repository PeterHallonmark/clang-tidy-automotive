//===--- UnstructuredSwitchStmtCheck.h - clang-tidy -------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_UNSTRUCTUREDSWITCHSTMTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_UNSTRUCTUREDSWITCHSTMTCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @brief Detects improperly structured switch statements.
///
/// Switch statements should be well-formed with cases appearing only at the
/// top level of the switch body. Nested cases, cases inside loops, or other
/// unstructured patterns make code harder to understand and maintain.
///
/// Example:
/// @code
///   switch (x) {
///     case 1:
///       if (y) {
///         case 2:  // Warning: case inside if
///           break;
///       }
///       break;
///   }
/// @endcode
class UnstructuredSwitchStmtCheck : public ClangTidyCheck {
public:
  UnstructuredSwitchStmtCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_UNSTRUCTUREDSWITCHSTMTCHECK_H
