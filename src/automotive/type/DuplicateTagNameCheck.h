//===--- DuplicateTagNameCheck.h - clang-tidy ------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_DUPLICATETAGNAMECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_DUPLICATETAGNAMECHECK_H

#include "../../ClangTidyCheck.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/StringMap.h"
#include <vector>

namespace clang::tidy::automotive {

/// Detects duplicate tag names (struct, union, enum) in a translation unit.
///
/// A tag name shall be a unique identifier within the translation unit.
/// Using the same tag name for different struct, union, or enum types creates
/// confusion and violates the principle that identifiers should have unique
/// meanings. This check ensures that each tag name is used only once.
///
/// Related MISRA C:2023 Rule 5.7 - A tag name shall be a unique identifier.
///
/// Example:
/// \code
///   struct Point { int x, y; };
///   enum Point { RED, GREEN };  // Warning: duplicate tag name 'Point'
/// \endcode
class DuplicateTagNameCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  DuplicateTagNameCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Registers AST matchers for tag declarations.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched tag declarations and tracks them.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

  /// Reports duplicate tag names at the end of translation unit processing.
  void onEndOfTranslationUnit() override;

private:
  /// Maps tag names to the list of declarations using that name
  llvm::StringMap<std::vector<const TagDecl *>> TagNames;
  /// Source manager pointer for end-of-TU reporting
  const SourceManager *SM = nullptr;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_DUPLICATETAGNAMECHECK_H
