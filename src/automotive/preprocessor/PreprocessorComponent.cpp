//===--- PreprocessorComponent.cpp - clang-tidy ---------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "PreprocessorComponent.h"
#include "AvoidCodeBeforeIncludeCheck.h"
#include "AvoidHashOperatorCheck.h"
#include "AvoidInvalidHeaderCharCheck.h"
#include "AvoidMacroNamedAsCkeywordCheck.h"
#include "AvoidMultipleHashOperatorsCheck.h"
#include "AvoidReservedMacroIdentifierCheck.h"
#include "AvoidUndefCheck.h"
#include "PreprocessorFlowCheck.h"
#include "UnusedMacroCheck.h"

namespace clang::tidy::automotive {

void PreprocessorComponent::addCheckFactories(
    ClangTidyCheckFactories &CheckFactories) {

  CheckFactories.registerCheck<AvoidMacroNamedAsCkeywordCheck>(
      "automotive-avoid-macro-named-as-ckeyword");
  CheckFactories.registerCheck<PreprocessorFlowCheck>("preprocessor-flow");

  CheckFactories.registerCheck<UnusedMacroCheck>("automotive-unused-macro");
  CheckFactories.registerCheck<AvoidUndefCheck>("automotive-avoid-undef");
  CheckFactories.registerCheck<AvoidHashOperatorCheck>(
      "automotive-avoid-hash-operator");
  CheckFactories.registerCheck<AvoidMultipleHashOperatorsCheck>(
      "automotive-avoid-multiple-hash-operators");

  // Rule 20.1 - Include directive order
  CheckFactories.registerCheck<AvoidCodeBeforeIncludeCheck>(
      "automotive-avoid-code-before-include");

  // Rule 20.2 - Invalid characters in header names
  CheckFactories.registerCheck<AvoidInvalidHeaderCharCheck>(
      "automotive-avoid-invalid-header-char");

  // Rule 21.1 - Reserved macro identifiers
  CheckFactories.registerCheck<AvoidReservedMacroIdentifierCheck>(
      "automotive-avoid-reserved-macro-identifier");
}

} // namespace clang::tidy::automotive
