//===--- StorageComponent.cpp - clang-tidy --------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "StorageComponent.h"
#include "AvoidGlobalObjectCheck.h"
#include "AvoidReservedIdentifierDeclarationCheck.h"
#include "AvoidRestrictTypeCheck.h"
#include "MissingStaticInternalLinkageCheck.h"

namespace clang::tidy::automotive {

void StorageComponent::addCheckFactories(
    ClangTidyCheckFactories &CheckFactories) {

  // Rule 21.2 - Reserved identifier declarations (Required)
  CheckFactories.registerCheck<AvoidReservedIdentifierDeclarationCheck>(
      "automotive-c23-req-21.2");

  // Rule 8.9 - Object at block scope (Advisory)
  CheckFactories.registerCheck<AvoidGlobalObjectCheck>(
      "automotive-c23-adv-8.9");

  CheckFactories.registerCheck<AvoidRestrictTypeCheck>(
      "automotive-avoid-restrict-type");
  CheckFactories.registerCheck<MissingStaticInternalLinkageCheck>(
      "automotive-missing-static-internal-linkage");
}

} // namespace clang::tidy::automotive
