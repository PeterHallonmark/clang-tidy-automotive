//===--- AvoidTrigraphCheck.cpp - clang-tidy -----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidTrigraphCheck.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"

namespace clang::tidy::automotive {

namespace {

class AvoidTrigraphPPCallbacks : public PPCallbacks {
public:
  AvoidTrigraphPPCallbacks(ClangTidyCheck &Check, const SourceManager &SM)
      : Check(Check), SM(SM) {}

  void FileChanged(SourceLocation Loc, FileChangeReason Reason,
                   SrcMgr::CharacteristicKind FileType,
                   FileID PrevFID) override {
    // Only scan the main file and included files
    if (Reason == EnterFile) {
      FileID FID = SM.getFileID(Loc);
      if (FID.isValid() && !ScannedFiles.count(FID)) {
        ScannedFiles.insert(FID);
        scanFileForTrigraphs(FID);
      }
    }
  }

  void EndOfMainFile() override {
    // Ensure main file is scanned
    FileID MainFileID = SM.getMainFileID();
    if (!ScannedFiles.count(MainFileID)) {
      scanFileForTrigraphs(MainFileID);
    }
  }

private:
  ClangTidyCheck &Check;
  const SourceManager &SM;
  llvm::DenseSet<FileID> ScannedFiles;

  void scanFileForTrigraphs(FileID FID) {
    bool Invalid = false;
    StringRef Buffer = SM.getBufferData(FID, &Invalid);
    if (Invalid)
      return;

    // Trigraph mapping table
    // Note: Using string literals to avoid trigraph warnings in this file
    static const struct {
      const char *Sequence;
      char Replacement;
    } Trigraphs[] = {
        {"\?\?=", '#'}, {"\?\?(", '['}, {"\?\?)", ']'}, {"\?\?<", '{'},
        {"\?\?>", '}'}, {"\?\?/", '\\'}, {"\?\?'", '^'}, {"\?\?!", '|'},
        {"\?\?-", '~'}
    };

    SourceLocation FileStart = SM.getLocForStartOfFile(FID);

    for (size_t Pos = 0; Pos < Buffer.size() - 2; ++Pos) {
      if (Buffer[Pos] == '?' && Buffer[Pos + 1] == '?') {
        char ThirdChar = Buffer[Pos + 2];

        // Check if this forms a valid trigraph
        char Replacement = '\0';
        for (const auto &TG : Trigraphs) {
          if (TG.Sequence[2] == ThirdChar) {
            Replacement = TG.Replacement;
            break;
          }
        }

        if (Replacement != '\0') {
          SourceLocation TrigraphLoc = FileStart.getLocWithOffset(Pos);

          // Format the warning message with the trigraph and its replacement
          SmallString<64> Message;
          llvm::raw_svector_ostream OS(Message);
          OS << "avoid trigraph sequence '??" << ThirdChar
             << "' (expands to '" << Replacement << "')";

          Check.diag(TrigraphLoc, Message);
        }
      }
    }
  }
};

} // anonymous namespace

void AvoidTrigraphCheck::registerPPCallbacks(const SourceManager &SM,
                                             Preprocessor *PP,
                                             Preprocessor *ModuleExpanderPP) {
  PP->addPPCallbacks(::std::make_unique<AvoidTrigraphPPCallbacks>(*this, SM));
}

} // namespace clang::tidy::automotive
