//===--- AvoidGlobalObjectCheck.cpp - clang-tidy --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidGlobalObjectCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidGlobalObjectCheck::registerMatchers(MatchFinder *Finder) {
  // Match file-scope static variables (internal linkage)
  // that are not const-qualified
  Finder->addMatcher(varDecl(hasGlobalStorage(), isStaticStorageClass(),
                             unless(hasAncestor(functionDecl())),
                             unless(isConstexpr()),
                             unless(hasType(qualType(isConstQualified()))))
                         .bind("staticvar"),
                     this);

  // Match references to static file-scope variables within functions
  // Filter out const variables as they are typically intentional constants
  Finder->addMatcher(
      declRefExpr(to(varDecl(hasGlobalStorage(), isStaticStorageClass(),
                             unless(hasAncestor(functionDecl())),
                             unless(hasType(qualType(isConstQualified()))))
                         .bind("refvar")),
                  hasAncestor(functionDecl().bind("func")))
          .bind("ref"),
      this);
}

void AvoidGlobalObjectCheck::check(const MatchFinder::MatchResult &Result) {
  SM = Result.SourceManager;

  // Handle static file-scope variable declarations
  if (const auto *StaticVar = Result.Nodes.getNodeAs<VarDecl>("staticvar")) {
    if (SM->isInSystemHeader(StaticVar->getLocation()))
      return;

    // Track this variable if not already tracked
    if (VarUsages.find(StaticVar) == VarUsages.end()) {
      VarUsages[StaticVar] = {};
      FileVars.push_back(StaticVar);
    }
    return;
  }

  // Handle references to static variables within functions
  if (Result.Nodes.getNodeAs<DeclRefExpr>("ref")) {
    const auto *RefVar = Result.Nodes.getNodeAs<VarDecl>("refvar");
    const auto *Func = Result.Nodes.getNodeAs<FunctionDecl>("func");

    if (!RefVar || !Func)
      return;

    if (SM->isInSystemHeader(RefVar->getLocation()))
      return;

    // Track which function uses this variable
    auto It = VarUsages.find(RefVar);
    if (It == VarUsages.end()) {
      VarUsages[RefVar] = {};
      FileVars.push_back(RefVar);
    }
    VarUsages[RefVar].insert(Func->getCanonicalDecl());
  }
}

void AvoidGlobalObjectCheck::onEndOfTranslationUnit() {
  for (const VarDecl *Var : FileVars) {
    auto It = VarUsages.find(Var);
    if (It == VarUsages.end())
      continue;

    const auto &Functions = It->second;

    // If used by exactly one function, suggest moving to block scope
    if (Functions.size() == 1) {
      const FunctionDecl *Func = *Functions.begin();
      diag(Var->getLocation(),
           "file-scope object '%0' is only used in function '%1'; "
           "consider defining it at block scope")
          << Var->getName() << Func->getName();
    }
  }

  // Clear for next TU
  VarUsages.clear();
  FileVars.clear();
}

} // namespace clang::tidy::automotive
