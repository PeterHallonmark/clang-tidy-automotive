//===--- ASTMatchers.h - clang-tidy -----------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_ASTMATCHERS_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_ASTMATCHERS_H

#include "clang/ASTMatchers/ASTMatchers.h"

namespace clang::tidy::automotive {

namespace ast_matchers {

using namespace clang::ast_matchers;

static bool isEssentiallyBooleanHelper(const Expr &Node);

AST_MATCHER(QualType, isRestrictType) {
  return Node.getLocalQualifiers().hasRestrict();
}

AST_MATCHER(SwitchStmt, hasDefaultStmt) {
  for (const SwitchCase *SC = Node.getSwitchCaseList(); SC;
       SC = SC->getNextSwitchCase()) {
    if (llvm::isa<DefaultStmt>(SC)) {
      return true;
    }
  }
  return false;
}

static bool isEssentiallyBooleanHelper(const Expr &Node) {
  QualType Type = Node.getType();

  // Direct Boolean type check (_Bool in C99+).
  if (Type->isBooleanType()) {
    return true;
  }

  // Handle binary operators that inherently return boolean values.
  // This includes comparison operators (==, !=, <, >, <=, >=)
  // and logical operators (&&, ||), which are commonly used in conditions.
  if (const auto *BinaryOp = dyn_cast<BinaryOperator>(&Node)) {
    return BinaryOp->isComparisonOp() || BinaryOp->isLogicalOp();
  }

  // Handle logical negation (!x), which is commonly used in conditional
  // expressions.
  if (const auto *UnaryOp = dyn_cast<UnaryOperator>(&Node)) {
    return UnaryOp->getOpcode() == UO_LNot;
  }

  // Handle parenthesized expressions, such as switch ((x == 0)).
  // Recursively check the inner expression.
  if (const auto *Paren = dyn_cast<ParenExpr>(&Node)) {
    return isEssentiallyBooleanHelper(*Paren->getSubExpr());
  }

  // Handle implicit casts to boolean, such as (bool)x.
  if (const auto *Cast = dyn_cast<ImplicitCastExpr>(&Node)) {
    return isEssentiallyBooleanHelper(*Cast->getSubExpr());
  }

  // The expression is essentially Boolean if both branches are Boolean.
  if (const auto *Cond = dyn_cast<ConditionalOperator>(&Node)) {
    return isEssentiallyBooleanHelper(*Cond->getTrueExpr()) &&
           isEssentiallyBooleanHelper(*Cond->getFalseExpr());
  }

  // If none of the above conditions match, the expression is not essentially
  // Boolean.
  return false;
}

AST_MATCHER(Expr, isEssentiallyBoolean) {
  return isEssentiallyBooleanHelper(Node);
}

AST_MATCHER(BinaryOperator, isAssignmentResultUsed) {
  const auto *Parent = Finder->getASTContext().getParents(Node)[0].get<Stmt>();
  if (!Parent) {
    return false;
  }
  return isa<Expr>(Parent);
}

AST_MATCHER_P(SwitchStmt, hasBody, clang::ast_matchers::internal::Matcher<Stmt>,
              InnerMatcher) {
  const Stmt *const Body = Node.getBody();
  return (Body != nullptr && InnerMatcher.matches(*Body, Finder, Builder));
}

AST_MATCHER(InitListExpr, isZeroInitializer) {
  return Node.getNumInits() == 1 &&
         llvm::isa<IntegerLiteral>(Node.getInit(0)) &&
         llvm::cast<IntegerLiteral>(Node.getInit(0))->getValue() == 0;
}

AST_MATCHER(InitListExpr, isStringLiteralInit) {
  return Node.isStringLiteralInit();
}

} // namespace ast_matchers

namespace automotive = ast_matchers;

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_ASTMATCHERS_H
