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

namespace clang::tidy {

class CustomDiagnostic {
public:
  CustomDiagnostic(StringRef CheckName, StringRef Message)
      : CheckName(CheckName), Message(Message) {}

  StringRef getCheckName() { return CheckName; }
  StringRef getMessage() { return Message; }

private:
  StringRef CheckName;
  StringRef Message;
};

class ClangTidyDiagnosticMapping : public DiagnosticConsumer {
public:
  ClangTidyDiagnosticMapping(ClangTidyContext &Context,
                             DiagnosticConsumer &DiagConsumer)
      : Context(Context), DiagConsumer(DiagConsumer) {}

  void clear() override;
  void BeginSourceFile(const LangOptions &LangOpts,
                       const Preprocessor *PP = nullptr) override;
  void EndSourceFile() override;
  void finish() override;
  bool IncludeInDiagnosticCounts() const override;
  void HandleDiagnostic(DiagnosticsEngine::Level DiagLevel,
                        const Diagnostic &Info) override;

private:
  ClangTidyContext &Context;
  DiagnosticConsumer &DiagConsumer;
};

} // namespace clang::tidy

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_CLANGTIDYDIAGNOSTICMAPPING_H
