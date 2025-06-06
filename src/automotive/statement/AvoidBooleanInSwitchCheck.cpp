//===--- AvoidBooleanInSwitchCheck.cpp - clang-tidy -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidBooleanInSwitchCheck.h"
#include "../utils/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidBooleanInSwitchCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      switchStmt(hasCondition(expr(automotive::isEssentiallyBoolean())))
          .bind("switchExpr"),
      this);
}

void AvoidBooleanInSwitchCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedSwitch = Result.Nodes.getNodeAs<SwitchStmt>("switchExpr");

  if (MatchedSwitch) {
    diag(MatchedSwitch->getBeginLoc(),
         "avoid boolean expression in switch statement");
  }
}

} // namespace clang::tidy::automotive
