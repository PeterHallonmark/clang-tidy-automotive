//===--- UnusedCodeModule.cpp - clang-tidy --------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "UnusedCodeModule.h"
#include "UnusedObjectDefinitionCheck.h"
#include "UnusedTagCheck.h"
#include "UnusedTypeCheck.h"

namespace clang::tidy::automotive {

void UnusedCodeModule::addCheckFactories(
    ClangTidyCheckFactories &CheckFactories) {

  /* C 2023
  ------------------------------------------------------------------ */
  CheckFactories.registerCheck<UnusedTypeCheck>("automotive-c23-adv-2.3");
  // CheckFactories.registerCheck<misc::UnusedParametersCheck>(
  //     "automotive-c23-adv-2.7");
  CheckFactories.registerCheck<UnusedObjectDefinitionCheck>(
      "automotive-c23-adv-2.8");

  /* C 2012
  ------------------------------------------------------------------ */
  CheckFactories.registerCheck<UnusedTypeCheck>("automotive-c12-adv-2.3");

  // CheckFactories.registerCheck<misc::UnusedParametersCheck>(
  //     "automotive-c12-adv-2.7");

  /* Experimental
  ------------------------------------------------------------------ */
  CheckFactories.registerCheck<UnusedTagCheck>("automotive-x-adv-2.4");
}

} // namespace clang::tidy::automotive
