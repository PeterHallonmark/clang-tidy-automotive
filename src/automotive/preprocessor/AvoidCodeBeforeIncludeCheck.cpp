//===--- AvoidCodeBeforeIncludeCheck.cpp - clang-tidy ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidCodeBeforeIncludeCheck.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {

class IncludeOrderPPCallbacks : public PPCallbacks {
public:
  IncludeOrderPPCallbacks(ClangTidyCheck &Check, const SourceManager &SM)
      : Check(Check), SM(SM), SeenCode(false) {}

  void InclusionDirective(SourceLocation HashLoc, const Token &IncludeTok,
                          StringRef FileName, bool IsAngled,
                          CharSourceRange FilenameRange,
                          OptionalFileEntryRef File, StringRef SearchPath,
                          StringRef RelativePath, const Module *SuggestedModule,
                          bool ModuleImported,
                          SrcMgr::CharacteristicKind FileType) override {
    // Only check in the main file
    if (!SM.isInMainFile(HashLoc))
      return;

    if (SeenCode) {
      Check.diag(HashLoc, "#include directive should only be preceded by "
                          "preprocessor directives or comments");
    }
  }

  void MacroDefined(const Token &MacroNameTok,
                    const MacroDirective *MD) override {
    // Macro definitions are preprocessor directives, so they're OK
    // But we track if we're past the include section
  }

  void FileChanged(SourceLocation Loc, FileChangeReason Reason,
                   SrcMgr::CharacteristicKind FileType,
                   FileID PrevFID) override {
    // Reset state when entering a new file
    if (Reason == EnterFile && SM.isInMainFile(Loc)) {
      SeenCode = false;
    }
  }

  void setSeenCode() { SeenCode = true; }

private:
  ClangTidyCheck &Check;
  const SourceManager &SM;
  bool SeenCode;
};

} // anonymous namespace

void AvoidCodeBeforeIncludeCheck::registerPPCallbacks(
    const SourceManager &SM, Preprocessor *PP, Preprocessor *ModuleExpanderPP) {
  PP->addPPCallbacks(std::make_unique<IncludeOrderPPCallbacks>(*this, SM));
}

} // namespace clang::tidy::automotive
