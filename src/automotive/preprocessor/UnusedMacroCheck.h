//===--- UnusedMacroCheck.h - clang-tidy ------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_UNUSEDMACROCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_UNUSEDMACROCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-preprocessor
/// @brief Detects macros that are defined but never used.
///
/// Unused macros indicate dead code that should be removed. They may be
/// remnants of old code, debugging aids left behind, or incomplete
/// implementations that were never finished.
///
/// @par MISRA C:2025 Rule 2.5
/// A project should not contain unused macro declarations.
/// @par Category: Advisory
///
/// Example:
/// @code
///   #define UNUSED_MACRO 42  // Warning: macro never used
///
///   #define USED_MACRO 100
///   int x = USED_MACRO;      // OK: macro is used
/// @endcode
class UnusedMacroCheck : public ClangTidyCheck {
public:
  UnusedMacroCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_UNUSEDMACROCHECK_H
