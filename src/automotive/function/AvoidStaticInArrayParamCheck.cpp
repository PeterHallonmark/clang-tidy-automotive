//===--- AvoidStaticInArrayParamCheck.cpp - clang-tidy --------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidStaticInArrayParamCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Lexer.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidStaticInArrayParamCheck::registerMatchers(MatchFinder *Finder) {
  // Match function declarations with parameters
  Finder->addMatcher(
      functionDecl(hasAnyParameter(parmVarDecl().bind("param"))).bind("func"),
      this);
}

void AvoidStaticInArrayParamCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *Param = Result.Nodes.getNodeAs<ParmVarDecl>("param");
  if (!Param)
    return;

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(Param->getLocation()))
    return;

  // Get the source range of the parameter
  SourceRange ParamRange = Param->getSourceRange();
  if (ParamRange.isInvalid())
    return;

  const SourceManager &SM = *Result.SourceManager;
  const LangOptions &LangOpts = Result.Context->getLangOpts();

  // Get the source text for this parameter
  CharSourceRange CharRange = CharSourceRange::getTokenRange(ParamRange);
  StringRef ParamText = Lexer::getSourceText(CharRange, SM, LangOpts);

  if (ParamText.empty())
    return;

  // Look for [static pattern in the parameter declaration
  // Valid patterns: [static N], [static N], [static sizeof(x)], etc.
  size_t BracketPos = ParamText.find('[');
  if (BracketPos == StringRef::npos)
    return;

  // Check if 'static' follows the opening bracket (allowing whitespace)
  StringRef AfterBracket = ParamText.substr(BracketPos + 1).ltrim();
  if (AfterBracket.starts_with("static")) {
    // Verify it's actually the keyword (not part of identifier)
    StringRef AfterStatic = AfterBracket.substr(6);
    if (AfterStatic.empty() || !std::isalnum(AfterStatic[0])) {
      diag(Param->getLocation(),
           "array parameter '%0' uses 'static' keyword which is not permitted "
           "by MISRA C")
          << Param->getName();
    }
  }
}

} // namespace clang::tidy::automotive
