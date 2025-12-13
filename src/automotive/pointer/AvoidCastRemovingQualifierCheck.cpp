//===--- AvoidCastRemovingQualifierCheck.cpp - clang-tidy -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidCastRemovingQualifierCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidCastRemovingQualifierCheck::registerMatchers(MatchFinder *Finder) {
  // Match C-style casts and explicit casts
  Finder->addMatcher(cStyleCastExpr().bind("cast"), this);
  Finder->addMatcher(cxxStaticCastExpr().bind("cast"), this);
  Finder->addMatcher(cxxReinterpretCastExpr().bind("cast"), this);
  Finder->addMatcher(cxxConstCastExpr().bind("cast"), this);
}

void AvoidCastRemovingQualifierCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *Cast = Result.Nodes.getNodeAs<ExplicitCastExpr>("cast");
  if (!Cast)
    return;

  // Skip casts in system headers
  if (Result.SourceManager->isInSystemHeader(Cast->getBeginLoc()))
    return;

  QualType SourceType = Cast->getSubExpr()->getType();
  QualType DestType = Cast->getType();

  // We only care about pointer types
  if (!SourceType->isPointerType() || !DestType->isPointerType())
    return;

  // Get the pointed-to types
  QualType SourcePointee = SourceType->getPointeeType();
  QualType DestPointee = DestType->getPointeeType();

  // Check for removed qualifiers
  bool RemovedConst = SourcePointee.isConstQualified() &&
                      !DestPointee.isConstQualified();
  bool RemovedVolatile = SourcePointee.isVolatileQualified() &&
                         !DestPointee.isVolatileQualified();

  if (RemovedConst && RemovedVolatile) {
    diag(Cast->getBeginLoc(),
         "cast removes 'const' and 'volatile' qualifiers from pointer type");
  } else if (RemovedConst) {
    diag(Cast->getBeginLoc(),
         "cast removes 'const' qualifier from pointer type");
  } else if (RemovedVolatile) {
    diag(Cast->getBeginLoc(),
         "cast removes 'volatile' qualifier from pointer type");
  }
}

} // namespace clang::tidy::automotive
