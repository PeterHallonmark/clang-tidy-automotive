//===--- AvoidPointerIntegerCastCheck.cpp - clang-tidy --------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidPointerIntegerCastCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidPointerIntegerCastCheck::registerMatchers(MatchFinder *Finder) {
  // Match casts from pointer to integer
  Finder->addMatcher(castExpr(hasSourceExpression(hasType(pointerType())),
                              hasType(isInteger()))
                         .bind("ptrToInt"),
                     this);

  // Match casts from integer to pointer
  Finder->addMatcher(castExpr(hasSourceExpression(hasType(isInteger())),
                              hasType(pointerType()))
                         .bind("intToPtr"),
                     this);
}

void AvoidPointerIntegerCastCheck::check(
    const MatchFinder::MatchResult &Result) {
  // Handle pointer to integer cast
  if (const auto *Cast = Result.Nodes.getNodeAs<CastExpr>("ptrToInt")) {
    // Skip if in system header
    if (Result.SourceManager->isInSystemHeader(Cast->getBeginLoc()))
      return;

    // Skip implicit casts that are part of normal operations
    if (isa<ImplicitCastExpr>(Cast) &&
        Cast->getCastKind() != CK_PointerToIntegral)
      return;

    // Only report explicit casts or pointer-to-integral conversions
    if (Cast->getCastKind() == CK_PointerToIntegral ||
        isa<CStyleCastExpr>(Cast) || isa<CXXReinterpretCastExpr>(Cast)) {
      diag(Cast->getBeginLoc(), "cast from pointer type %0 to integer type %1")
          << Cast->getSubExpr()->getType() << Cast->getType();
    }
    return;
  }

  // Handle integer to pointer cast
  if (const auto *Cast = Result.Nodes.getNodeAs<CastExpr>("intToPtr")) {
    // Skip if in system header
    if (Result.SourceManager->isInSystemHeader(Cast->getBeginLoc()))
      return;

    // Skip implicit casts
    if (isa<ImplicitCastExpr>(Cast) &&
        Cast->getCastKind() != CK_IntegralToPointer)
      return;

    // Skip null pointer constant (0 or NULL)
    const Expr *SubExpr = Cast->getSubExpr()->IgnoreParenImpCasts();
    if (SubExpr->isNullPointerConstant(*Result.Context,
                                       Expr::NPC_ValueDependentIsNull))
      return;

    // Only report explicit casts or integral-to-pointer conversions
    if (Cast->getCastKind() == CK_IntegralToPointer ||
        isa<CStyleCastExpr>(Cast) || isa<CXXReinterpretCastExpr>(Cast)) {
      diag(Cast->getBeginLoc(), "cast from integer type %0 to pointer type %1")
          << Cast->getSubExpr()->getType() << Cast->getType();
    }
  }
}

} // namespace clang::tidy::automotive
