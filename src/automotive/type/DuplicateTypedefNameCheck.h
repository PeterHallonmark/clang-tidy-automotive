//===--- DuplicateTypedefNameCheck.h - clang-tidy --------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_DUPLICATETYPEDEFNAMECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_DUPLICATETYPEDEFNAMECHECK_H

#include "../../ClangTidyCheck.h"
#include "llvm/ADT/DenseMap.h"

namespace clang::tidy::automotive {

/// Detects duplicate typedef names within a translation unit.
///
/// A typedef name shall be a unique identifier. Using the same name for
/// multiple typedef declarations makes the code confusing and can lead to
/// maintenance issues. Even if the typedefs resolve to the same underlying
/// type, having duplicate names reduces code clarity and may indicate
/// design issues.
///
/// Related MISRA C:2023 Rule: 5.6 - A typedef name shall be a unique
/// identifier.
///
/// Example:
/// \code
///   typedef int MyInt;
///   typedef float MyInt;  // Warning: duplicate typedef name 'MyInt'
/// \endcode
class DuplicateTypedefNameCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  DuplicateTypedefNameCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Registers AST matchers for typedef declarations.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched typedef declarations and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

  /// Called at the end of the translation unit to detect duplicates.
  void onEndOfTranslationUnit() override;

private:
  /// Map to track typedef names and their first declaration location
  llvm::DenseMap<llvm::StringRef, const TypedefDecl *> SeenTypedefs;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_DUPLICATETYPEDEFNAMECHECK_H
