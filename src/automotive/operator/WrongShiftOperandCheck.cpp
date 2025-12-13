//===--- WrongShiftOperandCheck.cpp - clang-tidy --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "WrongShiftOperandCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void WrongShiftOperandCheck::registerMatchers(MatchFinder *Finder) {
  // Match left and right shift operators
  Finder->addMatcher(
      binaryOperator(hasAnyOperatorName("<<", ">>", "<<=", ">>=")).bind("shift"),
      this);
}

void WrongShiftOperandCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *ShiftOp = Result.Nodes.getNodeAs<BinaryOperator>("shift");
  if (!ShiftOp)
    return;

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(ShiftOp->getOperatorLoc()))
    return;

  const Expr *LHS = ShiftOp->getLHS()->IgnoreParenImpCasts();
  const Expr *RHS = ShiftOp->getRHS()->IgnoreParenImpCasts();

  // Get the type of the left operand to determine bit width
  QualType LHSType = LHS->getType();
  if (LHSType.isNull())
    return;

  // Get the bit width of the left operand type
  const ASTContext &Ctx = *Result.Context;
  uint64_t TypeBitWidth = Ctx.getTypeSize(LHSType);

  // Try to evaluate the right operand as a constant
  Expr::EvalResult EvalResult;
  if (!RHS->EvaluateAsInt(EvalResult, Ctx))
    return; // Can't evaluate at compile time, skip

  llvm::APSInt ShiftAmount = EvalResult.Val.getInt();

  // Check for negative shift amount
  if (ShiftAmount.isNegative()) {
    diag(RHS->getBeginLoc(),
         "shift amount is negative (%0), which is undefined behavior")
        << ShiftAmount.toString(10);
    return;
  }

  // Check for shift amount >= bit width
  if (ShiftAmount.getZExtValue() >= TypeBitWidth) {
    diag(RHS->getBeginLoc(),
         "shift amount (%0) is greater than or equal to the width of the "
         "type (%1 bits)")
        << ShiftAmount.toString(10) << static_cast<unsigned>(TypeBitWidth);
  }
}

} // namespace clang::tidy::automotive
