//===--- TypeComponent.cpp - clang-tidy -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "TypeComponent.h"
#include "AvoidUnionCheck.h"
#include "ImplicitIntCheck.h"
#include "UniqueEnumValuesCheck.h"
#include "UnusedTypeCheck.h"

namespace clang::tidy::automotive {

void TypeComponent::addCheckFactories(ClangTidyCheckFactories &CheckFactories) {

  CheckFactories.registerCheck<AvoidUnionCheck>("automotive-avoid-union");
  CheckFactories.registerCheck<ImplicitIntCheck>("automotive-implicit-int");
  CheckFactories.registerCheck<UniqueEnumValuesCheck>(
      "automotive-unique-enum-value");
  CheckFactories.registerCheck<UnusedTypeCheck>("automotive-unused-type");
}

} // namespace clang::tidy::automotive
