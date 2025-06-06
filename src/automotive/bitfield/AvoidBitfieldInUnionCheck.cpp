//===--- AvoidBitfieldInUnionCheck.cpp - clang-tidy -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidBitfieldInUnionCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidBitfieldInUnionCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(recordDecl(allOf(isUnion(), has(fieldDecl(isBitField()))))
                         .bind("unionWithBitfield"),
                     this);
}

void AvoidBitfieldInUnionCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedDecl =
      Result.Nodes.getNodeAs<RecordDecl>("unionWithBitfield");

  if (MatchedDecl) {
    diag(MatchedDecl->getLocation(), "avoid bitfield in union");
  }
}

} // namespace clang::tidy::automotive
