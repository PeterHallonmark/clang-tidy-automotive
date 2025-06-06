//===--- AvoidNonBooleanInConditionCheck.cpp - clang-tidy -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidNonBooleanInConditionCheck.h"
#include "../utils/ASTMatchers.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidNonBooleanInConditionCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      ifStmt(hasCondition(expr(unless(automotive::isEssentiallyBoolean()))
                              .bind("condition")))
          .bind("ifStmt"),
      this);
  Finder->addMatcher(
      whileStmt(hasCondition(expr(unless(automotive::isEssentiallyBoolean()))
                                 .bind("condition")))
          .bind("whileStmt"),
      this);
  Finder->addMatcher(
      forStmt(hasCondition(expr(unless(automotive::isEssentiallyBoolean()))
                               .bind("condition")))
          .bind("forStmt"),
      this);
}

void AvoidNonBooleanInConditionCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *MatchedCond = Result.Nodes.getNodeAs<Expr>("condition");

  if (MatchedCond) {
    diag(MatchedCond->getBeginLoc(),
         "avoid using non-boolean expression in control flow condition");
  }
}

} // namespace clang::tidy::automotive
