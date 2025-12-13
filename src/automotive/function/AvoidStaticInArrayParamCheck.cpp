//===--- AvoidStaticInArrayParamCheck.cpp - clang-tidy --------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidStaticInArrayParamCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidStaticInArrayParamCheck::registerMatchers(MatchFinder *Finder) {
  // Match function declarations with parameters
  Finder->addMatcher(functionDecl(hasAnyParameter(parmVarDecl().bind("param")))
                         .bind("func"),
                     this);
}

void AvoidStaticInArrayParamCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *Param = Result.Nodes.getNodeAs<ParmVarDecl>("param");
  if (!Param)
    return;

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(Param->getLocation()))
    return;

  // Check if the parameter has a decay-adjusted type that was originally an
  // array with static
  QualType ParamType = Param->getOriginalType();

  // Check for DecayedType which indicates array-to-pointer decay
  if (const auto *DT = ParamType->getAs<DecayedType>()) {
    QualType OriginalType = DT->getOriginalType();

    // Check if the original type is a variable array type with static
    if (const auto *VAT =
            dyn_cast<VariableArrayType>(OriginalType.getTypePtr())) {
      // VariableArrayType with static has the static specifier
      if (VAT->getIndexTypeQualifiers().hasQualifiers() ||
          VAT->getSizeModifier() == ArraySizeModifier::Static) {
        diag(Param->getLocation(),
             "array parameter '%0' uses 'static' keyword which is not "
             "permitted")
            << Param->getName();
        return;
      }
    }

    // Check for ConstantArrayType with static modifier
    if (const auto *CAT =
            dyn_cast<ConstantArrayType>(OriginalType.getTypePtr())) {
      if (CAT->getSizeModifier() == ArraySizeModifier::Static) {
        diag(Param->getLocation(),
             "array parameter '%0' uses 'static' keyword which is not "
             "permitted")
            << Param->getName();
        return;
      }
    }

    // Check for IncompleteArrayType - it shouldn't have static, but check
    // anyway
    if (const auto *IAT =
            dyn_cast<IncompleteArrayType>(OriginalType.getTypePtr())) {
      if (IAT->getSizeModifier() == ArraySizeModifier::Static) {
        diag(Param->getLocation(),
             "array parameter '%0' uses 'static' keyword which is not "
             "permitted")
            << Param->getName();
      }
    }
  }
}

} // namespace clang::tidy::automotive
