//===--- AvoidSignalHeaderCheck.cpp - clang-tidy --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidSignalHeaderCheck.h"
// #include "AvoidApiPPCallbacks.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

namespace clang::tidy::automotive {

using namespace clang::ast_matchers;

static const StringRef ForbiddenFunctionNames[] = {
    "bsd_signal",  "kill",       "killpg",    "pthread_kill", "pthread_sigmask",
    "raise",       "sigaction",  "sigaddset", "sigaltstack",  "sigdelset",
    "sigemptyset", "sigfillset", "sighold",   "sigignore",    "siginterrupt",
    "sigismember", "signal",     "sigpause",  "sigpending",   "sigprocmask",
    "sigqueue",    "sigrelse",   "sigset",    "sigsuspend",   "sigtimedwait",
    "sigwait",     "sigwaitinfo"};

void AvoidSignalHeaderCheck::registerPPCallbacks(
    const SourceManager &SM, Preprocessor *PP, Preprocessor *ModuleExpanderPP) {
  // TODO: Add preprocessor support for the forbidden functions.
  //  PP->addPPCallbacks(std::make_unique<AvoidApiPPCallbacks>(
  //      *this, *PP, "signal.h", ForbiddenFunctionNames));
}

void AvoidSignalHeaderCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      callExpr(callee(functionDecl(hasAnyName(ForbiddenFunctionNames))))
          .bind("forbiddenFunctionCall"),
      this);
}

void AvoidSignalHeaderCheck::check(const MatchFinder::MatchResult &Result) {
  if (const auto *Call =
          Result.Nodes.getNodeAs<CallExpr>("forbiddenFunctionCall")) {
    const FunctionDecl *Func = Call->getDirectCallee();
    if (Func) {
      diag(Call->getBeginLoc(), "Avoid function '%0' from <signal.h>")
          << Func->getName();
    }
  }
}

} // namespace clang::tidy::automotive
