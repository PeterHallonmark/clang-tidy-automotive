//===--- ClangTidyDiagnosticMapping.h - clang-tidy --------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_CLANGTIDYDIAGNOSTICMAPPING_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_CLANGTIDYDIAGNOSTICMAPPING_H

#include "ClangTidyDiagnosticConsumer.h"
#include "clang/Basic/Diagnostic.h"
#include "llvm/ADT/DenseMap.h"

namespace clang::tidy {

class ClangTidyCustomDiagnostic {
public:
  ClangTidyCustomDiagnostic(StringRef CheckName, StringRef Message)
      : CheckName(CheckName), Message(Message) {}

  StringRef getCheckName() const { return CheckName; }
  StringRef getMessage() const { return Message; }

private:
  StringRef CheckName;
  StringRef Message;
};

class ClangTidyCustomDiagnosticEntry {
public:
  void addDiagnostic(std::unique_ptr<ClangTidyCustomDiagnostic> D) {
    Diagnostics.push_back(std::move(D));
  }

  bool keepOriginalDiagnostic() const {
    // TODO: Add a real condition here.
    return true;
  }

  llvm::ArrayRef<std::unique_ptr<ClangTidyCustomDiagnostic>>
  getDiagnostics() const {
    return Diagnostics;
  }

private:
  std::vector<std::unique_ptr<ClangTidyCustomDiagnostic>> Diagnostics;
};

class ClangTidyDiagnosticMapping : public DiagnosticConsumer {
public:
  ClangTidyDiagnosticMapping(ClangTidyContext &Context,
                             DiagnosticConsumer &DiagConsumer);

  void clear() override;
  void BeginSourceFile(const LangOptions &LangOpts,
                       const Preprocessor *PP = nullptr) override;
  void EndSourceFile() override;
  void finish() override;
  bool IncludeInDiagnosticCounts() const override;
  void HandleDiagnostic(DiagnosticsEngine::Level DiagLevel,
                        const Diagnostic &Info) override;

  void
  addCustomDiagnostic(StringRef CheckName,
                      std::unique_ptr<ClangTidyCustomDiagnostic> Diagnostic);

private:
  ClangTidyContext &Context;
  DiagnosticConsumer &DiagConsumer;
  llvm::DenseMap<StringRef, ClangTidyCustomDiagnosticEntry> DiagnosticMapping;
};

} // namespace clang::tidy

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_CLANGTIDYDIAGNOSTICMAPPING_H
