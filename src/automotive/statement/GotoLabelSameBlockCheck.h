//===--- GotoLabelSameBlockCheck.h - clang-tidy ----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GOTOLABELSAMEBLOCKCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GOTOLABELSAMEBLOCKCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-statements
/// @brief Detects goto statements where the target label is in a different
/// block scope.
///
/// A goto statement and its corresponding label must be in the same block to
/// ensure that the control flow remains clear and predictable. This check
/// verifies that both the goto statement and its target label share the same
/// immediate compound statement (block) as their parent.
///
/// @par MISRA C:2023 Rule 15.2
/// A goto statement and its corresponding label shall be in the same block.
/// @par Category: Required
///
/// Example:
/// @code
///   void f(int x) {
///     if (x > 0) {
///       goto label;  // Warning: label is in different block
///     }
///   label:  // label is at function scope, not in if block
///     return;
///   }
/// @endcode
///
/// Compliant example:
/// @code
///   void f(int x) {
///     if (x > 0) {
///       goto label;
///     label:  // Both goto and label in same if block
///       return;
///     }
///   }
/// @endcode
class GotoLabelSameBlockCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  GotoLabelSameBlockCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Registers AST matchers for goto statements.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched goto statements and emits diagnostics if the target
  /// label is in a different block.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GOTOLABELSAMEBLOCKCHECK_H
