//===--- AvoidInappropriateEssentialTypeCheck.h - clang-tidy ----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDINAPPROPRIATEESSENTIALTYPECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDINAPPROPRIATEESSENTIALTYPECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @brief Detects inappropriate essential type mixing in operations.
///
/// This check enforces the Essential Type Model which
/// categorizes types into Boolean, Character, Signed, Unsigned, Floating,
/// and Enum, and detects inappropriate mixing of these categories.
///
/// Common violations detected:
/// - Boolean types used in arithmetic operations
/// - Character types used in arithmetic expressions (not as character data)
/// - Inappropriate mixing of essential type categories
///
/// Example:
/// @code
///   bool b = true;
///   int x = b + 1;         // Warning: boolean in arithmetic
///   char c = 'A';
///   int y = c * 2;         // Warning: character in arithmetic
///   unsigned u = 1;
///   int z = u + b;         // Warning: boolean in arithmetic
/// @endcode
class AvoidInappropriateEssentialTypeCheck : public ClangTidyCheck {
public:
  AvoidInappropriateEssentialTypeCheck(StringRef Name,
                                       ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  /// Essential type categories based on MISRA Essential Type Model
  enum class EssentialType {
    Boolean,
    Character,
    SignedInt,
    UnsignedInt,
    FloatingPoint,
    Enum,
    Other
  };

  /// Determine the essential type category of a QualType
  EssentialType getEssentialType(QualType Type) const;

  /// Check if a type is appropriate for arithmetic operations
  bool isAppropriateForArithmetic(EssentialType ET) const;

  /// Check if a type is appropriate for bitwise operations
  bool isAppropriateForBitwise(EssentialType ET) const;

  /// Get a human-readable name for an essential type
  StringRef getEssentialTypeName(EssentialType ET) const;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDINAPPROPRIATEESSENTIALTYPECHECK_H
