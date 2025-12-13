//===--- FunctionComponent.cpp - clang-tidy -------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "FunctionComponent.h"
#include "AvoidFunctionParameterModificationCheck.h"
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
}

} // namespace clang::tidy::automotive
