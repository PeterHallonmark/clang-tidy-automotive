//===--- ForwardGotoLabelCheck.h - clang-tidy -------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_FORWARDGOTOLABELCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_FORWARDGOTOLABELCHECK_H

#include "../../ClangTidyCheck.h"
#include "clang/Basic/SourceLocation.h"
#include "llvm/ADT/DenseMap.h"

namespace clang::tidy::automotive {

struct GotoInfo {
  const GotoStmt *MatchedLabel = nullptr;
};

/// @ingroup misra-c25-statements
/// @brief Detects goto statements that jump backward in code.
///
/// Backward jumps with goto create loops that are harder to understand and
/// analyze than structured loop constructs (for, while, do-while). This check
/// ensures goto statements only jump forward if they must be used at all.
///
/// @par MISRA C:2025 Rule 15.3
/// Any label referenced by a goto statement shall be declared in the same
/// block, or in any block enclosing the goto statement.
/// @par Category: Required
///
/// Example:
/// @code
///   label:
///     x++;
///     if (x < 10)
///       goto label;  // Warning: backward jump
/// @endcode
class ForwardGotoLabelCheck : public ClangTidyCheck {
public:
  ForwardGotoLabelCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_FORWARDGOTOLABELCHECK_H
