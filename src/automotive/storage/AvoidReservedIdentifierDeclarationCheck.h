//===--- AvoidReservedIdentifierDeclarationCheck.h - clang-tidy -*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDRESERVEDIDENTIFIERDECLARATIONCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDRESERVEDIDENTIFIERDECLARATIONCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-storage
/// @brief Detects declarations that use reserved identifiers.
///
/// Reserved identifiers (those starting with underscore followed by uppercase
/// or another underscore, or identifiers reserved by the C standard library)
/// shall not be declared. Such declarations may cause undefined behavior
/// and conflict with compiler or library internals.
///
/// @par MISRA C:2025 Rule 21.2
/// A reserved identifier or macro name shall not be declared.
/// @par Category: Required
///
/// Example:
/// @code
///   int _Reserved;      // Warning: starts with underscore + uppercase
///   void __internal();  // Warning: starts with double underscore
///   int errno;          // Warning: standard library name
///   typedef int size_t; // Warning: standard library type
/// @endcode
class AvoidReservedIdentifierDeclarationCheck : public ClangTidyCheck {
public:
  AvoidReservedIdentifierDeclarationCheck(StringRef Name,
                                          ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDRESERVEDIDENTIFIERDECLARATIONCHECK_H
