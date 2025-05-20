//===--- UncompleteFunctionPrototypeCheck.cpp - clang-tidy ----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "UncompleteFunctionPrototypeCheck.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::misra {

void UncompleteFunctionPrototypeCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(functionDecl().bind("func"), this);
}

void UncompleteFunctionPrototypeCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *MatchedFunc = Result.Nodes.getNodeAs<FunctionDecl>("func");

  if (MatchedFunc) {
    if (!MatchedFunc->hasPrototype()) {
      diag(MatchedFunc->getLocation(), "function is not in prototype form");
    }

    for (const ParmVarDecl *Param : MatchedFunc->parameters()) {
      if (Param->getName().empty()) {
        diag(Param->getLocation(), "function parameter is unnamed");
      }
    }

    if (MatchedFunc->parameters().empty() &&
        !MatchedFunc->hasWrittenPrototype()) {
      diag(MatchedFunc->getLocation(),
           "function with no parameters must use 'void' in prototype");
    }
  }
}

} // namespace clang::tidy::misra
