//===--- StdlibComponent.cpp - clang-tidy ---------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "StdlibComponent.h"
#include "AtoXCheck.h"
#include "AvoidSetjmpHeaderCheck.h"
#include "AvoidSignalHeaderCheck.h"
#include "AvoidStdargHeaderCheck.h"
#include "AvoidStdlibMemoryCheck.h"
#include "AvoidStdlibRandCheck.h"
#include "AvoidstdlibsystemcallCheck.h"
#include "ExitCheck.h"

namespace clang::tidy::automotive {

void StdlibComponent::addCheckFactories(
    ClangTidyCheckFactories &CheckFactories) {

  CheckFactories.registerCheck<AvoidStdargHeaderCheck>(
      "automotive-avoid-stdarg-header");
  // CheckFactories.registerCheck<AvoidStdlibMemoryCheck>(
  //     "automotive-avoid-stdlib-malloc");
  CheckFactories.registerCheck<AvoidSetjmpHeaderCheck>(
      "automotive-avoid-setjmp-header");
  CheckFactories.registerCheck<AvoidSignalHeaderCheck>(
      "automotive-avoid-signal-header");
  CheckFactories.registerCheck<AtoXCheck>("automotive-avoid-ascii-to-number");
  CheckFactories.registerCheck<ExitCheck>("automotive-avoid-stdlib-exit");

  CheckFactories.registerCheck<AvoidstdlibsystemcallCheck>(
      "automotive-avoid-stdlib-system");
  CheckFactories.registerCheck<AvoidStdlibRandCheck>(
      "automotive-avoid-stdlib-rand");
}

} // namespace clang::tidy::automotive
