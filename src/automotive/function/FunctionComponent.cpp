//===--- FunctionComponent.cpp - clang-tidy -------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "FunctionComponent.h"
#include "AvoidFunctionParameterModificationCheck.h"
#include "AvoidStaticInArrayParamCheck.h"
#include "FunctionDeclarationMismatchCheck.h"
#include "ImplicitFunctionDeclCheck.h"
#include "StaticInlineCheck.h"
#include "UncompleteFunctionPrototypeCheck.h"

namespace clang::tidy::automotive {

void FunctionComponent::addCheckFactories(
    ClangTidyCheckFactories &CheckFactories) {

  CheckFactories.registerCheck<UncompleteFunctionPrototypeCheck>(
      "automotive-uncomplete-function-prototype");
  CheckFactories.registerCheck<FunctionDeclarationMismatchCheck>(
      "automotive-function-declaration-mismatch");
  CheckFactories.registerCheck<StaticInlineCheck>("automotive-missing-static-inline");

  CheckFactories.registerCheck<ImplicitFunctionDeclCheck>(
      "automotive-implicit-function-decl");
  CheckFactories.registerCheck<AvoidFunctionParameterModificationCheck>(
      "automotive-avoid-function-parameter-modification");

  // Rule 17.6 - Static keyword in array parameters (Mandatory)
  CheckFactories.registerCheck<AvoidStaticInArrayParamCheck>(
      "automotive-c23-mand-17.6");
}

} // namespace clang::tidy::automotive
