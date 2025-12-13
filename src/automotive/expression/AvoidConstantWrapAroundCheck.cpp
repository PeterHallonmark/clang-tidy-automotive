//===--- AvoidConstantWrapAroundCheck.cpp - clang-tidy -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidConstantWrapAroundCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Expr.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "llvm/ADT/APInt.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {

/// Visitor to check for wrap-around in binary and unary operations
class WrapAroundChecker : public RecursiveASTVisitor<WrapAroundChecker> {
public:
  explicit WrapAroundChecker(ASTContext &Context) : Context(Context) {}

  bool foundWrapAround() const { return FoundWrapAround; }
  SourceLocation getWrapAroundLoc() const { return WrapAroundLoc; }

  bool VisitBinaryOperator(BinaryOperator *BO) {
    if (!BO->getType()->isUnsignedIntegerType())
      return true;

    // Check for subtraction where LHS < RHS (will wrap)
    if (BO->getOpcode() == BO_Sub) {
      Expr::EvalResult LHS, RHS;
      if (BO->getLHS()->EvaluateAsRValue(LHS, Context) &&
          BO->getRHS()->EvaluateAsRValue(RHS, Context)) {
        if (LHS.Val.isInt() && RHS.Val.isInt()) {
          if (LHS.Val.getInt().ult(RHS.Val.getInt())) {
            FoundWrapAround = true;
            WrapAroundLoc = BO->getOperatorLoc();
            return false;
          }
        }
      }
    }

    // Check for addition or multiplication that exceeds the type's max value
    if (BO->getOpcode() == BO_Add || BO->getOpcode() == BO_Mul) {
      Expr::EvalResult Result;
      if (BO->EvaluateAsRValue(Result, Context)) {
        if (Result.Val.isInt()) {
          Expr::EvalResult LHS, RHS;
          if (BO->getLHS()->EvaluateAsRValue(LHS, Context) &&
              BO->getRHS()->EvaluateAsRValue(RHS, Context)) {
            if (LHS.Val.isInt() && RHS.Val.isInt()) {
              const llvm::APSInt &LHSVal = LHS.Val.getInt();
              const llvm::APSInt &RHSVal = RHS.Val.getInt();
              const llvm::APSInt &ResultVal = Result.Val.getInt();

              // For addition: if result < either operand, wrap occurred
              if (BO->getOpcode() == BO_Add) {
                if (ResultVal.ult(LHSVal) || ResultVal.ult(RHSVal)) {
                  FoundWrapAround = true;
                  WrapAroundLoc = BO->getOperatorLoc();
                  return false;
                }
              }
              // For multiplication: check if result/lhs != rhs (simplified overflow check)
              else if (BO->getOpcode() == BO_Mul) {
                if (!LHSVal.isZero() && !RHSVal.isZero()) {
                  // Check if we can divide back and get the original value
                  llvm::APSInt Quotient = ResultVal.udiv(LHSVal);
                  if (Quotient != RHSVal) {
                    FoundWrapAround = true;
                    WrapAroundLoc = BO->getOperatorLoc();
                    return false;
                  }
                }
              }
            }
          }
        }
      }
    }

    return true;
  }

  bool VisitUnaryOperator(UnaryOperator *UO) {
    // Unary minus on unsigned type (except for 0)
    if (UO->getOpcode() == UO_Minus &&
        UO->getType()->isUnsignedIntegerType()) {
      Expr::EvalResult Operand;
      if (UO->getSubExpr()->EvaluateAsRValue(Operand, Context)) {
        if (Operand.Val.isInt() && !Operand.Val.getInt().isZero()) {
          FoundWrapAround = true;
          WrapAroundLoc = UO->getOperatorLoc();
          return false;
        }
      }
    }
    return true;
  }

private:
  ASTContext &Context;
  bool FoundWrapAround = false;
  SourceLocation WrapAroundLoc;
};

} // anonymous namespace

void AvoidConstantWrapAroundCheck::registerMatchers(MatchFinder *Finder) {
  // Match variable declarations with initializers that are constant expressions
  Finder->addMatcher(
      varDecl(hasInitializer(expr().bind("init")), isDefinition()).bind("var"),
      this);
}

void AvoidConstantWrapAroundCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *VD = Result.Nodes.getNodeAs<VarDecl>("var");
  const auto *InitExpr = Result.Nodes.getNodeAs<Expr>("init");

  if (!VD || !InitExpr)
    return;

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(VD->getLocation()))
    return;

  // Only check const variables or those in file scope (likely constants)
  if (!VD->getType().isConstQualified() && !VD->hasGlobalStorage())
    return;

  // Only check unsigned integer types
  QualType Type = VD->getType();
  if (!Type->isUnsignedIntegerType())
    return;

  // Check if the initializer can be evaluated as a constant
  Expr::EvalResult EvalResult;
  if (!InitExpr->EvaluateAsRValue(EvalResult, *Result.Context))
    return;

  // Walk the initializer to find wrap-around operations
  WrapAroundChecker Checker(*Result.Context);
  Checker.TraverseStmt(const_cast<Expr *>(InitExpr));

  if (Checker.foundWrapAround()) {
    diag(Checker.getWrapAroundLoc(),
         "constant expression evaluation leads to unsigned integer wrap-around");
  }
}

} // namespace clang::tidy::automotive
