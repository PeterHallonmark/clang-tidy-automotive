//===--- MissingExternalArraySizeCheck.cpp - clang-tidy -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MissingExternalArraySizeCheck.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void MissingExternalArraySizeCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(varDecl(hasType(arrayType()), hasExternalFormalLinkage(),
                             hasType(incompleteArrayType()))
                         .bind("array"),
                     this);
}

void MissingExternalArraySizeCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *MatchedDecl = Result.Nodes.getNodeAs<VarDecl>("array");

  if (MatchedDecl) {
    diag(MatchedDecl->getLocation(),
         "missing explicit size of external array '%0'")
        << MatchedDecl->getName();
  }
}

} // namespace clang::tidy::automotive
