//===--- PreprocessorComponent.cpp - clang-tidy ---------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "PreprocessorComponent.h"

namespace clang::tidy::misra {

void PreprocessorComponent::addCheckFactories(
    ClangTidyCheckFactories &CheckFactories) {

  /* MISRA C 2023
  ------------------------------------------------------------------ */

  /* MISRA C 2012
  ------------------------------------------------------------------ */
}

} // namespace clang::tidy::misra
