//===--- DuplicateTagNameCheck.cpp - clang-tidy --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "DuplicateTagNameCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void DuplicateTagNameCheck::registerMatchers(MatchFinder *Finder) {
  // Match all tag declarations (struct, union, enum) that have a name
  // Exclude implicit declarations and system headers
  Finder->addMatcher(
      tagDecl(unless(isImplicit()), unless(isExpansionInSystemHeader()))
          .bind("tag"),
      this);
}

void DuplicateTagNameCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Tag = Result.Nodes.getNodeAs<TagDecl>("tag");
  SM = Result.SourceManager;

  if (!Tag)
    return;

  // Skip anonymous tags (no name)
  if (Tag->getName().empty())
    return;

  // Skip if in system header (double check)
  if (SM->isInSystemHeader(Tag->getLocation()))
    return;

  // Get the canonical declaration to avoid counting forward declarations
  // and definitions of the same tag as duplicates
  const TagDecl *CanonicalTag = Tag->getCanonicalDecl();

  // Check if we've already seen this canonical declaration
  StringRef TagName = CanonicalTag->getName();
  auto &Decls = TagNames[TagName];

  // Only add if we haven't seen this exact canonical declaration before
  bool AlreadyTracked = false;
  for (const TagDecl *ExistingDecl : Decls) {
    if (ExistingDecl == CanonicalTag) {
      AlreadyTracked = true;
      break;
    }
  }

  if (!AlreadyTracked) {
    Decls.push_back(CanonicalTag);
  }
}

void DuplicateTagNameCheck::onEndOfTranslationUnit() {
  // Check for duplicate tag names
  for (const auto &Entry : TagNames) {
    const auto &Decls = Entry.getValue();

    // If more than one canonical declaration uses this name, it's a duplicate
    if (Decls.size() > 1) {
      // Report the first duplicate and subsequent ones
      for (size_t i = 1; i < Decls.size(); ++i) {
        const TagDecl *CurrentTag = Decls[i];
        const TagDecl *FirstTag = Decls[0];

        // Get tag type name for better diagnostics
        StringRef TagKind;
        if (isa<EnumDecl>(CurrentTag))
          TagKind = "enum";
        else if (const auto *RD = dyn_cast<RecordDecl>(CurrentTag))
          TagKind = RD->isUnion() ? "union" : "struct";

        StringRef FirstTagKind;
        if (isa<EnumDecl>(FirstTag))
          FirstTagKind = "enum";
        else if (const auto *RD = dyn_cast<RecordDecl>(FirstTag))
          FirstTagKind = RD->isUnion() ? "union" : "struct";

        diag(CurrentTag->getLocation(),
             "duplicate tag name '%0'; %1 '%0' conflicts with %2 declared here")
            << Entry.getKey() << TagKind << FirstTagKind;
        diag(FirstTag->getLocation(), "previous declaration of %0 '%1' is here",
             DiagnosticIDs::Note)
            << FirstTagKind << Entry.getKey();
      }
    }
  }

  // Clear for next TU
  TagNames.clear();
}

} // namespace clang::tidy::automotive
