//===--- tools/extra/clang-tidy/ClangTidyDiagnosticMapping.cpp -----------=== //
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "ClangTidyDiagnosticMapping.h"
#include "ClangTidy.h"
#include "clang/Basic/DiagnosticIDs.h"

namespace clang::tidy {

void ClangTidyDiagnosticMapping::clear() { DiagConsumer.clear(); }

void ClangTidyDiagnosticMapping::BeginSourceFile(const LangOptions &LangOpts,
                                                 const Preprocessor *PP) {
  DiagConsumer.BeginSourceFile(LangOpts, PP);
}

void ClangTidyDiagnosticMapping::EndSourceFile() {
  DiagConsumer.EndSourceFile();
}

void ClangTidyDiagnosticMapping::finish() { DiagConsumer.finish(); }

bool ClangTidyDiagnosticMapping::IncludeInDiagnosticCounts() const {
  return DiagConsumer.IncludeInDiagnosticCounts();
}

void ClangTidyDiagnosticMapping::HandleDiagnostic(
    DiagnosticsEngine::Level DiagLevel, const Diagnostic &Info) {
  
  if (Context.DiagEngine) {
      Context.DiagEngine->setClient(&DiagConsumer, false);

      Context.diag("testing", Info.getLocation(), "testing testing");

      Context.DiagEngine->setClient(this, false);
      
      llvm::outs() << Info.getID() << " " << Context.getCheckName(Info.getID())
                   << "\n";

  } else {
    llvm::outs() << "Fail: " << Info.getID() << " " << "\n";
  }

  DiagConsumer.HandleDiagnostic(DiagLevel, Info);
  NumWarnings = DiagConsumer.getNumWarnings();
  NumErrors = DiagConsumer.getNumErrors();
}

} // namespace clang::tidy
