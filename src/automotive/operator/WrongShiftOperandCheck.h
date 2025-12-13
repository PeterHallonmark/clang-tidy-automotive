//===--- WrongShiftOperandCheck.h - clang-tidy ------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_WRONGSHIFTOPERANDCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_WRONGSHIFTOPERANDCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-operator
/// @brief Detects shift operators with out-of-bounds shift amounts.
///
/// The right-hand operand of a shift operator shall lie in the range zero to
/// one less than the width in bits of the essential type of the left-hand
/// operand. Shifting by negative amounts or by amounts equal to or greater
/// than the bit width results in undefined behavior.
///
/// @par MISRA C:2025 Rule 12.2
/// The right hand operand of a shift operator shall lie in the range zero to
/// one less than the width in bits of the essential type of the left hand
/// operand.
/// @par Category: Required
///
/// Example:
/// @code
///   uint32_t x = 1;
///   uint32_t y = x << 32;  // Warning: shift amount equals bit width
///   uint32_t z = x << -1;  // Warning: negative shift amount
///   uint8_t  a = 1;
///   uint8_t  b = a << 8;   // Warning: shift amount equals bit width of uint8_t
/// @endcode
class WrongShiftOperandCheck : public ClangTidyCheck {
public:
  WrongShiftOperandCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_WRONGSHIFTOPERANDCHECK_H
