//===--- UnusedObjectDefinitionCheck.h - clang-tidy -------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_UNUSEDOBJECTDEFINITIONCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_UNUSEDOBJECTDEFINITIONCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-unused-code
/// @brief Detects unused object (variable) definitions.
///
/// This check identifies variables that are defined but never used in the code.
/// Unused objects may indicate incomplete implementations, dead code, or
/// refactoring remnants that should be cleaned up.
///
/// @par MISRA C:2025 Rule 2.8
/// A variable should be used after being declared.
/// @par Category: Advisory
///
/// Example:
/// @code
///   void func(void) {
///     int unused_var = 42;  // Warning: unused variable
///   }
/// @endcode
class UnusedObjectDefinitionCheck : public ClangTidyCheck {
public:
  UnusedObjectDefinitionCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_UNUSEDOBJECTDEFINITIONCHECK_H
