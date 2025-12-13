//===--- UnusedParameterCheck.cpp - clang-tidy ----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "UnusedParameterCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void UnusedParameterCheck::registerMatchers(MatchFinder *Finder) {
  // Match function definitions with parameters
  Finder->addMatcher(functionDecl(isDefinition(), hasBody(compoundStmt()),
                                  unless(isImplicit()), unless(isDefaulted()),
                                  unless(isDeleted()))
                         .bind("func"),
                     this);
}

void UnusedParameterCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Func = Result.Nodes.getNodeAs<FunctionDecl>("func");
  if (!Func)
    return;

  // Skip functions in system headers
  if (Result.SourceManager->isInSystemHeader(Func->getLocation()))
    return;

  // Check each parameter
  for (const ParmVarDecl *Param : Func->parameters()) {
    // Skip unnamed parameters - they're explicitly unused
    if (!Param->getIdentifier())
      continue;

    // Skip parameters that are used
    if (Param->isUsed())
      continue;

    // Skip if the parameter is referenced (covers more cases than isUsed)
    if (Param->isReferenced())
      continue;

    // Report unused parameter
    diag(Param->getLocation(), "unused parameter '%0'") << Param->getName();
  }
}

} // namespace clang::tidy::automotive
