//===--- GotoLabelSameBlockCheck.cpp - clang-tidy ------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "GotoLabelSameBlockCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ParentMapContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

/// Helper function to get the LabelStmt from a GotoStmt
static const LabelStmt *getLabelStmt(const GotoStmt *Goto) {
  if (Goto) {
    const auto *Label = Goto->getLabel();
    if (Label) {
      return Label->getStmt();
    }
  }
  return nullptr;
}

/// Helper function to find the immediate parent CompoundStmt for a given Stmt
/// Returns nullptr if no CompoundStmt parent is found
static const CompoundStmt *
findImmediateCompoundStmt(const Stmt *S, ASTContext &Context) {
  if (!S)
    return nullptr;

  DynTypedNodeList Parents = Context.getParents(*S);
  while (!Parents.empty()) {
    // Get the first parent
    const DynTypedNode &Parent = Parents[0];

    // Check if this parent is a CompoundStmt
    if (const auto *CS = Parent.get<CompoundStmt>()) {
      return CS;
    }

    // Move up to the next parent
    Parents = Context.getParents(Parent);
  }

  return nullptr;
}

void GotoLabelSameBlockCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(gotoStmt().bind("goto"), this);
}

void GotoLabelSameBlockCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedGoto = Result.Nodes.getNodeAs<GotoStmt>("goto");
  if (!MatchedGoto)
    return;

  const auto *MatchedLabel = getLabelStmt(MatchedGoto);
  if (!MatchedLabel)
    return;

  ASTContext &Context = *Result.Context;

  // Find the immediate parent CompoundStmt for both goto and label
  const auto *GotoBlock = findImmediateCompoundStmt(MatchedGoto, Context);
  const auto *LabelBlock = findImmediateCompoundStmt(MatchedLabel, Context);

  // If either doesn't have a CompoundStmt parent, or they're different blocks
  if (!GotoBlock || !LabelBlock || GotoBlock != LabelBlock) {
    diag(MatchedGoto->getGotoLoc(),
         "goto statement and its label '%0' are not in the same block")
        << MatchedLabel->getName();
    diag(MatchedLabel->getBeginLoc(), "location of label '%0'",
         DiagnosticIDs::Note)
        << MatchedLabel->getName();
  }
}

} // namespace clang::tidy::automotive
