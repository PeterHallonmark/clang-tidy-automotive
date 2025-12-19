//===--- UnusedParameterCheck.h - clang-tidy --------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_UNUSEDPARAMETERCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_UNUSEDPARAMETERCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @brief Detects unused function parameters.
///
/// Function parameters that are not used in the function body may indicate
/// incomplete implementations, interface mismatches, or dead code. Unused
/// parameters should be removed or marked to suppress the warning if they
/// are intentionally unused (e.g., for API compatibility).
///
/// Example:
/// @code
///   void func(int used, int unused) {  // Warning: 'unused' is not used
///       return used + 1;
///   }
///
///   // Compliant: cast to void to indicate intentionally unused
///   void callback(int event, void *data) {
///       (void)data;  // Intentionally unused
///       handle_event(event);
///   }
/// @endcode
class UnusedParameterCheck : public ClangTidyCheck {
public:
  UnusedParameterCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_UNUSEDPARAMETERCHECK_H
