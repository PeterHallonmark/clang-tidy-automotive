//===--- ExpressionComponent.cpp - clang-tidy -----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "ExpressionComponent.h"
#include "AvoidAssignmentInExpressionCheck.h"
#include "AvoidNonBooleanInConditionCheck.h"
#include "AvoidSideEffectInLogicalOperandCheck.h"
#include "AvoidSideEffectInSizeofCheck.h"
#include "InvariantControlCheck.h"

namespace clang::tidy::automotive {

void ExpressionComponent::addCheckFactories(
    ClangTidyCheckFactories &CheckFactories) {

  // Rule 13.4 - Assignment used as expression (Advisory)
  CheckFactories.registerCheck<AvoidAssignmentInExpressionCheck>(
      "automotive-c23-adv-13.4");

  // Rule 13.5 - Side effects in logical operator operands (Required)
  CheckFactories.registerCheck<AvoidSideEffectInLogicalOperandCheck>(
      "automotive-c23-req-13.5");

  // Rule 13.6 - Side effects in sizeof operand (Mandatory)
  CheckFactories.registerCheck<AvoidSideEffectInSizeofCheck>(
      "automotive-c23-mand-13.6");

  // Rule 14.3 - Invariant controlling expression (Required)
  CheckFactories.registerCheck<InvariantControlCheck>(
      "automotive-c23-req-14.3");

  // Rule 14.4 - Non-boolean in condition (Required)
  CheckFactories.registerCheck<AvoidNonBooleanInConditionCheck>(
      "automotive-c23-req-14.4");
}

} // namespace clang::tidy::automotive
