//===--- AutomotiveTidyCheck.h - clang-tidy ---------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AUTOMOTIVETIDYCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AUTOMOTIVETIDYCHECK_H

#include "../ClangTidyCheck.h"

namespace clang::tidy::automotive {

class AutomotiveTidyCheck : public ClangTidyCheck {
public:
  AutomotiveTidyCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  virtual void registerMatchers(ast_matchers::MatchFinder *Finder) override {}
  virtual void
  check(const ast_matchers::MatchFinder::MatchResult &Result) override {}
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AUTOMOTIVETIDYCHECK_H
