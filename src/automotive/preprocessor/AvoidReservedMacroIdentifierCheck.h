//===--- AvoidReservedMacroIdentifierCheck.h - clang-tidy -------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDRESERVEDMACROIDENTIFIERCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDRESERVEDMACROIDENTIFIERCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @brief Detects #define and #undef of reserved identifiers.
///
/// Reserved identifiers (those starting with underscore followed by uppercase
/// or another underscore, and standard library names) shall not be defined or
/// undefined. Modifying these can cause undefined behavior and break the
/// standard library.
///
/// Example:
/// @code
///   #define _RESERVED 1     // Warning: starts with underscore + uppercase
///   #define __internal 2    // Warning: starts with double underscore
///   #define errno 0         // Warning: standard library name
///   #undef NULL             // Warning: standard library name
/// @endcode
class AvoidReservedMacroIdentifierCheck : public ClangTidyCheck {
public:
  AvoidReservedMacroIdentifierCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDRESERVEDMACROIDENTIFIERCHECK_H
