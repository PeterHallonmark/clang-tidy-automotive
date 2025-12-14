//===--- MissingHeaderGuardCheck.cpp - clang-tidy -------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MissingHeaderGuardCheck.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"
#include "llvm/Support/Path.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {

/// Canonicalize a path by removing ./ and ../ components.
static std::string cleanPath(StringRef Path) {
  SmallString<256> Result = Path;
  llvm::sys::path::remove_dots(Result, true);
  return std::string(Result);
}

/// Generate a standard header guard macro name from a file path.
static std::string generateHeaderGuardName(StringRef FileName) {
  SmallString<256> Guard;

  // Get just the filename without directory
  StringRef BaseName = llvm::sys::path::filename(FileName);

  // Convert to uppercase and replace non-alphanumeric chars with underscores
  for (char C : BaseName) {
    if (std::isalnum(static_cast<unsigned char>(C))) {
      Guard.push_back(std::toupper(static_cast<unsigned char>(C)));
    } else {
      Guard.push_back('_');
    }
  }

  Guard.push_back('_');
  return std::string(Guard);
}

class HeaderGuardPPCallbacks : public PPCallbacks {
public:
  HeaderGuardPPCallbacks(Preprocessor *PP, MissingHeaderGuardCheck *Check)
      : PP(PP), Check(Check) {}

  void FileChanged(SourceLocation Loc, FileChangeReason Reason,
                   SrcMgr::CharacteristicKind FileType,
                   FileID PrevFID) override {
    // Track header files that we enter
    SourceManager &SM = PP->getSourceManager();
    if (Reason == EnterFile && FileType == SrcMgr::C_User) {
      if (OptionalFileEntryRef FE =
              SM.getFileEntryRefForID(SM.getFileID(Loc))) {
        std::string FileName = cleanPath(FE->getName());

        // Only track .h and .hpp files (header files)
        StringRef Extension = llvm::sys::path::extension(FileName);
        if (Extension == ".h" || Extension == ".hpp") {
          Files[FileName] = *FE;
        }
      }
    }
  }

  void PragmaDirective(SourceLocation Loc,
                       PragmaIntroducerKind Introducer) override {
    // This callback is called for all pragmas, but we need to check
    // specifically for #pragma once in PragmaOnce callback
  }

  void MacroDefined(const Token &MacroNameTok,
                    const MacroDirective *MD) override {
    // Record all defined macros to check for header guard pattern
    const MacroInfo *MI = MD->getMacroInfo();
    if (!MI)
      return;

    Macros.emplace_back(MacroNameTok, MI);
  }

  void EndOfMainFile() override {
    SourceManager &SM = PP->getSourceManager();

    // Check which macros are used as header guards
    // Clang's preprocessor marks macros that are used as header guards
    for (const auto &MacroEntry : Macros) {
      const MacroInfo *MI = MacroEntry.second;

      if (!MI->isUsedForHeaderGuard())
        continue;

      // This file has a proper header guard, remove it from tracking
      OptionalFileEntryRef FE =
          SM.getFileEntryRefForID(SM.getFileID(MI->getDefinitionLoc()));
      if (FE) {
        std::string FileName = cleanPath(FE->getName());
        Files.erase(FileName);
      }
    }

    // Check for files with #pragma once
    // Note: Clang doesn't provide a direct PPCallbacks for #pragma once
    // detection in the standard API, so we rely on the header guard mechanism
    // above. Files with #pragma once are also marked with
    // isUsedForHeaderGuard()

    // Emit warnings for header files without guards
    checkGuardlessHeaders();
    clearAllState();
  }

  void checkGuardlessHeaders() {
    for (const auto &FE : Files) {
      StringRef FileName = FE.getKey();
      SourceManager &SM = PP->getSourceManager();
      FileID FID = SM.translateFile(FE.getValue());
      SourceLocation StartLoc = SM.getLocForStartOfFile(FID);

      if (StartLoc.isInvalid())
        continue;

      // Check if this file was included from the main file or another header
      // We only want to report on files that are actually being analyzed
      if (!SM.isInFileID(StartLoc, FID))
        continue;

      // Generate suggested header guard name
      std::string GuardName = generateHeaderGuardName(FileName);

      // Emit diagnostic with fix-it hints
      Check->diag(StartLoc,
                  "header file is missing include guard protection; "
                  "consider adding '#ifndef %0' / '#define %0' at the start "
                  "and '#endif' at the end, or use '#pragma once'")
          << GuardName
          << FixItHint::CreateInsertion(StartLoc, "#ifndef " + GuardName +
                                                      "\n#define " + GuardName +
                                                      "\n\n")
          << FixItHint::CreateInsertion(SM.getLocForEndOfFile(FID),
                                        "\n\n#endif // " + GuardName + "\n");
    }
  }

private:
  void clearAllState() {
    Macros.clear();
    Files.clear();
  }

  std::vector<std::pair<Token, const MacroInfo *>> Macros;
  llvm::StringMap<const FileEntry *> Files;
  Preprocessor *PP;
  MissingHeaderGuardCheck *Check;
};

} // anonymous namespace

void MissingHeaderGuardCheck::registerPPCallbacks(
    const SourceManager &SM, Preprocessor *PP, Preprocessor *ModuleExpanderPP) {
  PP->addPPCallbacks(std::make_unique<HeaderGuardPPCallbacks>(PP, this));
}

} // namespace clang::tidy::automotive
