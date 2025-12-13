//===--- AvoidSideEffectInLogicalOperandCheck.cpp - clang-tidy ------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidSideEffectInLogicalOperandCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {

/// Visitor to detect side effects in an expression tree.
class SideEffectFinder : public RecursiveASTVisitor<SideEffectFinder> {
public:
  bool hasSideEffect() const { return HasSideEffect; }

  bool VisitUnaryOperator(UnaryOperator *UO) {
    // Detect ++, -- operators
    if (UO->isIncrementDecrementOp()) {
      HasSideEffect = true;
      return false; // Stop traversal
    }
    return true;
  }

  bool VisitBinaryOperator(BinaryOperator *BO) {
    // Detect assignment operators (=, +=, -=, etc.)
    if (BO->isAssignmentOp()) {
      HasSideEffect = true;
      return false;
    }
    return true;
  }

  bool VisitCompoundAssignOperator(CompoundAssignOperator *) {
    HasSideEffect = true;
    return false;
  }

  bool VisitCallExpr(CallExpr *CE) {
    // Function calls are considered to have potential side effects
    // unless we can prove they're pure (which is complex to determine)
    HasSideEffect = true;
    return false;
  }

  bool VisitDeclRefExpr(DeclRefExpr *DRE) {
    // Check for volatile variable access
    if (DRE->getType().isVolatileQualified()) {
      HasSideEffect = true;
      return false;
    }
    return true;
  }

private:
  bool HasSideEffect = false;
};

} // anonymous namespace

void AvoidSideEffectInLogicalOperandCheck::registerMatchers(
    MatchFinder *Finder) {
  // Match logical && and || operators
  Finder->addMatcher(
      binaryOperator(hasAnyOperatorName("&&", "||")).bind("logical"), this);
}

void AvoidSideEffectInLogicalOperandCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *LogicalOp = Result.Nodes.getNodeAs<BinaryOperator>("logical");
  if (!LogicalOp)
    return;

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(LogicalOp->getOperatorLoc()))
    return;

  // Check the right-hand operand for side effects
  const Expr *RHS = LogicalOp->getRHS();
  if (!RHS)
    return;

  if (hasSideEffect(RHS)) {
    diag(RHS->getBeginLoc(),
         "right operand of '%0' operator contains side effects that may not "
         "be evaluated due to short-circuit evaluation")
        << LogicalOp->getOpcodeStr();
  }
}

bool AvoidSideEffectInLogicalOperandCheck::hasSideEffect(const Expr *E) const {
  SideEffectFinder Finder;
  Finder.TraverseStmt(const_cast<Expr *>(E));
  return Finder.hasSideEffect();
}

} // namespace clang::tidy::automotive
