//===--- AvoidInappropriateEssentialTypeCheck.cpp - clang-tidy -----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidInappropriateEssentialTypeCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidInappropriateEssentialTypeCheck::registerMatchers(
    MatchFinder *Finder) {
  // Match binary operators: arithmetic, bitwise, and relational
  // We focus on operators where essential type violations are most common
  Finder->addMatcher(
      binaryOperator(anyOf(
                         // Arithmetic operators
                         hasAnyOperatorName("+", "-", "*", "/", "%"),
                         // Bitwise operators
                         hasAnyOperatorName("&", "|", "^", "<<", ">>"),
                         // Compound assignment with arithmetic
                         hasAnyOperatorName("+=", "-=", "*=", "/=", "%=", "&=",
                                            "|=", "^=", "<<=", ">>=")))
          .bind("binop"),
      this);

  // Match unary operators that may have essential type issues
  Finder->addMatcher(
      unaryOperator(anyOf(
                        // Bitwise NOT
                        hasOperatorName("~"),
                        // Unary plus/minus
                        hasOperatorName("+"), hasOperatorName("-")))
          .bind("unop"),
      this);
}

void AvoidInappropriateEssentialTypeCheck::check(
    const MatchFinder::MatchResult &Result) {

  // Handle binary operators
  if (const auto *BinOp = Result.Nodes.getNodeAs<BinaryOperator>("binop")) {
    // Skip if in system header
    if (Result.SourceManager->isInSystemHeader(BinOp->getOperatorLoc()))
      return;

    // Get essential types of both operands, ignoring implicit casts
    // to get the underlying expression type
    const Expr *LHS = BinOp->getLHS()->IgnoreParenImpCasts();
    const Expr *RHS = BinOp->getRHS()->IgnoreParenImpCasts();

    QualType LHSType = LHS->getType();
    QualType RHSType = RHS->getType();

    EssentialType LHSET = getEssentialType(LHSType);
    EssentialType RHSET = getEssentialType(RHSType);

    bool IsArithmetic = BinOp->isAdditiveOp() || BinOp->isMultiplicativeOp();
    bool IsBitwise = BinOp->isBitwiseOp() || BinOp->isShiftOp();

    // Check for inappropriate types in arithmetic operations
    if (IsArithmetic) {
      if (!isAppropriateForArithmetic(LHSET)) {
        diag(LHS->getBeginLoc(),
             "operand of essential type '%0' is inappropriate for arithmetic "
             "operation")
            << getEssentialTypeName(LHSET);
      }
      if (!isAppropriateForArithmetic(RHSET)) {
        diag(RHS->getBeginLoc(),
             "operand of essential type '%0' is inappropriate for arithmetic "
             "operation")
            << getEssentialTypeName(RHSET);
      }
    }

    // Check for inappropriate types in bitwise operations
    if (IsBitwise) {
      if (!isAppropriateForBitwise(LHSET)) {
        diag(LHS->getBeginLoc(),
             "operand of essential type '%0' is inappropriate for bitwise "
             "operation")
            << getEssentialTypeName(LHSET);
      }
      if (!isAppropriateForBitwise(RHSET)) {
        diag(RHS->getBeginLoc(),
             "operand of essential type '%0' is inappropriate for bitwise "
             "operation")
            << getEssentialTypeName(RHSET);
      }
    }
  }

  // Handle unary operators
  if (const auto *UnOp = Result.Nodes.getNodeAs<UnaryOperator>("unop")) {
    // Skip if in system header
    if (Result.SourceManager->isInSystemHeader(UnOp->getOperatorLoc()))
      return;

    const Expr *SubExpr = UnOp->getSubExpr()->IgnoreParenImpCasts();
    QualType OpType = SubExpr->getType();
    EssentialType ET = getEssentialType(OpType);

    // Bitwise NOT
    if (UnOp->getOpcode() == UO_Not) {
      if (!isAppropriateForBitwise(ET)) {
        diag(SubExpr->getBeginLoc(),
             "operand of essential type '%0' is inappropriate for bitwise NOT "
             "operation")
            << getEssentialTypeName(ET);
      }
    }

    // Unary plus/minus
    if (UnOp->getOpcode() == UO_Plus || UnOp->getOpcode() == UO_Minus) {
      if (!isAppropriateForArithmetic(ET)) {
        diag(SubExpr->getBeginLoc(),
             "operand of essential type '%0' is inappropriate for unary '%1' "
             "operation")
            << getEssentialTypeName(ET)
            << (UnOp->getOpcode() == UO_Plus ? "+" : "-");
      }
    }
  }
}

AvoidInappropriateEssentialTypeCheck::EssentialType
AvoidInappropriateEssentialTypeCheck::getEssentialType(QualType Type) const {
  // Remove typedefs, const, volatile, etc. to get the canonical type
  Type = Type.getCanonicalType().getUnqualifiedType();

  // Boolean type
  if (Type->isBooleanType()) {
    return EssentialType::Boolean;
  }

  // Character types
  if (Type->isAnyCharacterType()) {
    return EssentialType::Character;
  }

  // Enumeration types
  if (Type->isEnumeralType()) {
    return EssentialType::Enum;
  }

  // Floating point types
  if (Type->isFloatingType()) {
    return EssentialType::FloatingPoint;
  }

  // Integer types - distinguish signed from unsigned
  if (Type->isIntegerType()) {
    if (Type->isUnsignedIntegerType()) {
      return EssentialType::UnsignedInt;
    } else if (Type->isSignedIntegerType()) {
      return EssentialType::SignedInt;
    }
  }

  return EssentialType::Other;
}

bool AvoidInappropriateEssentialTypeCheck::isAppropriateForArithmetic(
    EssentialType ET) const {
  // Boolean and Character types are inappropriate for arithmetic
  // Signed, Unsigned, Floating, and Enum are generally appropriate
  switch (ET) {
  case EssentialType::Boolean:
  case EssentialType::Character:
    return false;
  case EssentialType::SignedInt:
  case EssentialType::UnsignedInt:
  case EssentialType::FloatingPoint:
  case EssentialType::Enum:
    return true;
  case EssentialType::Other:
    return true; // Be conservative for unknown types
  }
  return true;
}

bool AvoidInappropriateEssentialTypeCheck::isAppropriateForBitwise(
    EssentialType ET) const {
  // Boolean, Character, and Floating types are inappropriate for bitwise
  // Signed, Unsigned integers are appropriate
  switch (ET) {
  case EssentialType::Boolean:
  case EssentialType::Character:
  case EssentialType::FloatingPoint:
    return false;
  case EssentialType::SignedInt:
  case EssentialType::UnsignedInt:
  case EssentialType::Enum:
    return true;
  case EssentialType::Other:
    return true; // Be conservative for unknown types
  }
  return true;
}

StringRef AvoidInappropriateEssentialTypeCheck::getEssentialTypeName(
    EssentialType ET) const {
  switch (ET) {
  case EssentialType::Boolean:
    return "Boolean";
  case EssentialType::Character:
    return "Character";
  case EssentialType::SignedInt:
    return "Signed";
  case EssentialType::UnsignedInt:
    return "Unsigned";
  case EssentialType::FloatingPoint:
    return "Floating";
  case EssentialType::Enum:
    return "Enum";
  case EssentialType::Other:
    return "Other";
  }
  return "Unknown";
}

} // namespace clang::tidy::automotive
