//===--- AvoidGlobalObjectCheck.h - clang-tidy ------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDGLOBALOBJECTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDGLOBALOBJECTCHECK_H

#include "../../ClangTidyCheck.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/SmallPtrSet.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-storage
/// @brief Detects file-scope objects that could be defined at block scope.
///
/// An object should be defined at block scope if its identifier only appears
/// in a single function. This improves locality and makes the code easier to
/// understand and maintain.
///
/// @par MISRA C:2025 Rule 8.9
/// An object should be defined at block scope if its identifier only appears
/// in a single function.
/// @par Category: Advisory
///
/// Example:
/// @code
///   static int counter;  // Warning if only used in one function
///
///   void increment() {
///       counter++;
///   }
///
///   // Better: move to block scope
///   void increment() {
///       static int counter = 0;  // OK: defined at block scope
///       counter++;
///   }
/// @endcode
class AvoidGlobalObjectCheck : public ClangTidyCheck {
public:
  AvoidGlobalObjectCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  void onEndOfTranslationUnit() override;

private:
  /// Maps file-scope variables to the functions that reference them
  llvm::DenseMap<const VarDecl *, llvm::SmallPtrSet<const FunctionDecl *, 4>>
      VarUsages;
  /// Store variable declarations for end-of-TU reporting
  llvm::SmallVector<const VarDecl *, 16> FileVars;
  /// Source manager pointer for end-of-TU reporting
  const SourceManager *SM = nullptr;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDGLOBALOBJECTCHECK_H
