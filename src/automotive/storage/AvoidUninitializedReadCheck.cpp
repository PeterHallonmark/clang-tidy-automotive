//===--- AvoidUninitializedReadCheck.cpp - clang-tidy --------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidUninitializedReadCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidUninitializedReadCheck::registerMatchers(MatchFinder *Finder) {
  // Match DeclRefExpr that refers to a local variable (automatic storage)
  // that does not have an initializer.
  // We look for variables in the initializer of other variables or in
  // expressions that are not themselves initializers.

  // Match uninitialized local variables being read
  // Exclude function parameters (ParmVarDecl) - they are initialized by caller
  Finder->addMatcher(declRefExpr(to(varDecl(hasAutomaticStorageDuration(),
                                            unless(hasInitializer(anything())),
                                            unless(parmVarDecl()))
                                        .bind("uninit_var")),
                                 // Ensure we're in a context where the variable
                                 // is being read (not just declared)
                                 unless(hasParent(declStmt())))
                         .bind("ref"),
                     this);
}

void AvoidUninitializedReadCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *Ref = Result.Nodes.getNodeAs<DeclRefExpr>("ref");
  const auto *Var = Result.Nodes.getNodeAs<VarDecl>("uninit_var");

  if (!Ref || !Var)
    return;

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(Ref->getLocation()))
    return;

  // Check if this is truly a read operation
  // We want to avoid false positives for address-of operations
  // or when the variable is on the left-hand side of an assignment

  // Get the parent context to determine if this is a read or write
  const auto Parents = Result.Context->getParents(*Ref);
  if (Parents.empty())
    return;

  const auto *Parent = Parents[0].get<Stmt>();
  if (!Parent)
    return;

  // If the parent is a unary operator & (address-of), this might be okay
  // as it could be an out-parameter being initialized
  if (const auto *UnaryOp = dyn_cast<UnaryOperator>(Parent)) {
    if (UnaryOp->getOpcode() == UO_AddrOf)
      return;
  }

  // If the parent is a binary operator and this is the LHS of an assignment,
  // it's being written to, not read
  if (const auto *BinOp = dyn_cast<BinaryOperator>(Parent)) {
    if (BinOp->isAssignmentOp() && BinOp->getLHS() == Ref)
      return;
  }

  // If we reach here, this appears to be a read of an uninitialized variable
  diag(Ref->getLocation(),
       "value of object '%0' with automatic storage duration is read "
       "before it has been initialized")
      << Var->getName();

  diag(Var->getLocation(), "variable '%0' declared here", DiagnosticIDs::Note)
      << Var->getName();
}

} // namespace clang::tidy::automotive
