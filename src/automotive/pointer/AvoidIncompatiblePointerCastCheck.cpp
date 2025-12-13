//===--- AvoidIncompatiblePointerCastCheck.cpp - clang-tidy ---------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidIncompatiblePointerCastCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidIncompatiblePointerCastCheck::registerMatchers(MatchFinder *Finder) {
  // Match C-style casts and reinterpret_cast
  Finder->addMatcher(
      castExpr(hasSourceExpression(hasType(pointerType())),
               hasType(pointerType()))
          .bind("cast"),
      this);
}

void AvoidIncompatiblePointerCastCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *Cast = Result.Nodes.getNodeAs<CastExpr>("cast");
  if (!Cast)
    return;

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(Cast->getBeginLoc()))
    return;

  // Skip implicit casts
  if (Cast->getCastKind() == CK_LValueToRValue ||
      Cast->getCastKind() == CK_NoOp ||
      Cast->getCastKind() == CK_ArrayToPointerDecay ||
      Cast->getCastKind() == CK_FunctionToPointerDecay)
    return;

  // Get the source and destination types
  QualType SrcType = Cast->getSubExpr()->getType();
  QualType DstType = Cast->getType();

  // Get pointee types
  const PointerType *SrcPtr = SrcType->getAs<PointerType>();
  const PointerType *DstPtr = DstType->getAs<PointerType>();

  if (!SrcPtr || !DstPtr)
    return;

  QualType SrcPointee = SrcPtr->getPointeeType().getUnqualifiedType();
  QualType DstPointee = DstPtr->getPointeeType().getUnqualifiedType();

  // Skip if same type (ignoring qualifiers)
  if (SrcPointee == DstPointee)
    return;

  // Allow casts to/from void*
  if (SrcPointee->isVoidType() || DstPointee->isVoidType())
    return;

  // Allow casts to char* or unsigned char* (commonly used for byte access)
  if (DstPointee->isCharType() ||
      (DstPointee->isSpecificBuiltinType(BuiltinType::UChar)))
    return;

  // Allow casts from char* or unsigned char*
  if (SrcPointee->isCharType() ||
      (SrcPointee->isSpecificBuiltinType(BuiltinType::UChar)))
    return;

  // Report incompatible pointer cast
  diag(Cast->getBeginLoc(),
       "cast from %0 to %1 converts between incompatible pointer types")
      << SrcType << DstType;
}

} // namespace clang::tidy::automotive
