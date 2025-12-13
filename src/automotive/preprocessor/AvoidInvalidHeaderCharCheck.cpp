//===--- AvoidInvalidHeaderCharCheck.cpp - clang-tidy ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidInvalidHeaderCharCheck.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {

class InvalidHeaderCharPPCallbacks : public PPCallbacks {
public:
  InvalidHeaderCharPPCallbacks(ClangTidyCheck &Check) : Check(Check) {}

  void InclusionDirective(SourceLocation HashLoc, const Token &IncludeTok,
                          StringRef FileName, bool IsAngled,
                          CharSourceRange FilenameRange,
                          OptionalFileEntryRef File, StringRef SearchPath,
                          StringRef RelativePath, const Module *SuggestedModule,
                          bool ModuleImported,
                          SrcMgr::CharacteristicKind FileType) override {
    // Check for invalid characters in the filename
    bool HasSingleQuote = FileName.contains('\'');
    bool HasDoubleQuote = FileName.contains('"');
    bool HasBackslash = FileName.contains('\\');

    if (HasSingleQuote || HasDoubleQuote || HasBackslash) {
      std::string InvalidChars;
      if (HasSingleQuote)
        InvalidChars += "'";
      if (HasDoubleQuote) {
        if (!InvalidChars.empty())
          InvalidChars += ", ";
        InvalidChars += "\"";
      }
      if (HasBackslash) {
        if (!InvalidChars.empty())
          InvalidChars += ", ";
        InvalidChars += "\\";
      }

      Check.diag(FilenameRange.getBegin(),
                 "header file name contains invalid character(s): %0")
          << InvalidChars;
    }
  }

private:
  ClangTidyCheck &Check;
};

} // anonymous namespace

void AvoidInvalidHeaderCharCheck::registerPPCallbacks(
    const SourceManager &SM, Preprocessor *PP, Preprocessor *ModuleExpanderPP) {
  PP->addPPCallbacks(std::make_unique<InvalidHeaderCharPPCallbacks>(*this));
}

} // namespace clang::tidy::automotive
