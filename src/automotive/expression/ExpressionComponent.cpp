//===--- ExpressionComponent.cpp - clang-tidy -----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "ExpressionComponent.h"

namespace clang::tidy::automotive {

void ExpressionComponent::addCheckFactories(
    ClangTidyCheckFactories &CheckFactories) {

#if 0
    /* 14.
    ---------------------------------------------------------------- */
    CheckFactories.registerCheck<InvariantControlCheck>(
        "automotive-c23-req-14.3");

    /* 13.
    ---------------------------------------------------------------- */
    CheckFactories.registerCheck<AvoidAssignmentInExpressionCheck>(
        "automotive-c23-adv-13.4");
    CheckFactories.registerCheck<AvoidNonBooleanInConditionCheck>(
        "automotive-c23-req-14.4");
#endif
}

} // namespace clang::tidy::automotive
