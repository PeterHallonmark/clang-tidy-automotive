//===--- AvoidSideEffectInSizeofCheck.cpp - clang-tidy --------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidSideEffectInSizeofCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {

/// Visitor to detect expressions with potential side effects
class SideEffectFinder : public RecursiveASTVisitor<SideEffectFinder> {
public:
  bool hasSideEffect() const { return FoundSideEffect; }
  SourceLocation getSideEffectLoc() const { return SideEffectLoc; }
  StringRef getSideEffectKind() const { return SideEffectKind; }

  bool VisitUnaryOperator(UnaryOperator *UO) {
    // ++, --, & (address-of on volatile)
    if (UO->isIncrementDecrementOp()) {
      FoundSideEffect = true;
      SideEffectLoc = UO->getOperatorLoc();
      SideEffectKind = UO->isIncrementOp() ? "increment" : "decrement";
      return false;
    }
    return true;
  }

  bool VisitBinaryOperator(BinaryOperator *BO) {
    // Assignment operators: =, +=, -=, etc.
    if (BO->isAssignmentOp()) {
      FoundSideEffect = true;
      SideEffectLoc = BO->getOperatorLoc();
      SideEffectKind = "assignment";
      return false;
    }
    return true;
  }

  bool VisitCompoundAssignOperator(CompoundAssignOperator *CAO) {
    FoundSideEffect = true;
    SideEffectLoc = CAO->getOperatorLoc();
    SideEffectKind = "compound assignment";
    return false;
  }

  bool VisitCallExpr(CallExpr *CE) {
    // Function calls may have side effects
    FoundSideEffect = true;
    SideEffectLoc = CE->getBeginLoc();
    SideEffectKind = "function call";
    return false;
  }

  bool VisitCXXNewExpr(CXXNewExpr *NE) {
    FoundSideEffect = true;
    SideEffectLoc = NE->getBeginLoc();
    SideEffectKind = "new expression";
    return false;
  }

  bool VisitCXXDeleteExpr(CXXDeleteExpr *DE) {
    FoundSideEffect = true;
    SideEffectLoc = DE->getBeginLoc();
    SideEffectKind = "delete expression";
    return false;
  }

private:
  bool FoundSideEffect = false;
  SourceLocation SideEffectLoc;
  StringRef SideEffectKind;
};

} // anonymous namespace

void AvoidSideEffectInSizeofCheck::registerMatchers(MatchFinder *Finder) {
  // Match sizeof expressions with an argument expression (not just a type)
  Finder->addMatcher(unaryExprOrTypeTraitExpr(ofKind(UETT_SizeOf),
                                              hasArgumentOfType(anything()))
                         .bind("sizeof"),
                     this);
}

void AvoidSideEffectInSizeofCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *SizeofExpr =
      Result.Nodes.getNodeAs<UnaryExprOrTypeTraitExpr>("sizeof");
  if (!SizeofExpr)
    return;

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(SizeofExpr->getBeginLoc()))
    return;

  // Check if this sizeof has an argument expression (not just a type)
  if (SizeofExpr->isArgumentType())
    return; // sizeof(type) - no side effects possible

  const Expr *ArgExpr = SizeofExpr->getArgumentExpr();
  if (!ArgExpr)
    return;

  // Visit the argument expression to find side effects
  SideEffectFinder Finder;
  Finder.TraverseStmt(const_cast<Expr *>(ArgExpr));

  if (Finder.hasSideEffect()) {
    diag(SizeofExpr->getBeginLoc(),
         "sizeof operand contains %0 which has potential side effects; "
         "these side effects will not be evaluated")
        << Finder.getSideEffectKind();
  }
}

} // namespace clang::tidy::automotive
