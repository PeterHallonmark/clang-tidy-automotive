//===--- AvoidMacroIdentifierConflictCheck.h - clang-tidy -------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDMACROIDENTIFIERCONFLICTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDMACROIDENTIFIERCONFLICTCHECK_H

#include "../../ClangTidyCheck.h"
#include "llvm/ADT/StringMap.h"
#include <string>

namespace clang::tidy::automotive {

/// @ingroup misra-c25-preprocessor
/// @brief Detects macro identifiers that conflict with other identifiers.
///
/// Macro identifiers shall be distinct from other identifiers (variables,
/// functions, types, labels) in the translation unit. Having the same name
/// for a macro and another identifier can lead to confusion and unexpected
/// behavior during preprocessing.
///
/// @par MISRA C:2023 Rule 5.4
/// Macro identifiers shall be distinct from other identifiers.
/// @par Category: Required
///
/// Example:
/// @code
///   #define MAX_SIZE 100
///   int MAX_SIZE;           // Warning: variable has same name as macro
///
///   #define calculate(x) ((x) * 2)
///   void calculate(int x);  // Warning: function has same name as macro
/// @endcode
class AvoidMacroIdentifierConflictCheck : public ClangTidyCheck {
public:
  AvoidMacroIdentifierConflictCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  void onEndOfTranslationUnit() override;

  // Storage for macro names and their locations
  struct MacroInfo {
    std::string Name;
    SourceLocation Location;
  };

  void addMacro(StringRef Name, SourceLocation Loc);
  void addIdentifier(StringRef Name, SourceLocation Loc, StringRef Type);

private:
  llvm::StringMap<SourceLocation> MacroNames;
  llvm::StringMap<std::pair<SourceLocation, std::string>> IdentifierNames;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDMACROIDENTIFIERCONFLICTCHECK_H
