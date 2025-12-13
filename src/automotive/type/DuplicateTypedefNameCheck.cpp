//===--- DuplicateTypedefNameCheck.cpp - clang-tidy ----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "DuplicateTypedefNameCheck.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void DuplicateTypedefNameCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(typedefDecl().bind("typedef"), this);
}

void DuplicateTypedefNameCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedDecl = Result.Nodes.getNodeAs<TypedefDecl>("typedef");

  if (!MatchedDecl) {
    return;
  }

  // Skip implicit typedef declarations
  if (MatchedDecl->isImplicit()) {
    return;
  }

  // Get the typedef name
  StringRef TypedefName = MatchedDecl->getName();

  // Check if we've seen this name before
  auto It = SeenTypedefs.find(TypedefName);
  if (It != SeenTypedefs.end()) {
    const TypedefDecl *FirstDecl = It->second;

    // Emit diagnostic for the duplicate
    diag(MatchedDecl->getLocation(),
         "duplicate typedef name '%0', previously declared")
        << TypedefName;
    diag(FirstDecl->getLocation(), "previous declaration is here",
         DiagnosticIDs::Note);
  } else {
    // First occurrence, store it
    SeenTypedefs[TypedefName] = MatchedDecl;
  }
}

void DuplicateTypedefNameCheck::onEndOfTranslationUnit() {
  // Clear the map for the next translation unit
  SeenTypedefs.clear();
}

} // namespace clang::tidy::automotive
