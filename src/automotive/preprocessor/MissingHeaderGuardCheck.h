//===--- MissingHeaderGuardCheck.h - clang-tidy ----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MISSINGHEADERGUARDCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MISSINGHEADERGUARDCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @brief Detects header files that lack proper include guard protection.
///
/// Header files should be protected against multiple inclusion to prevent
/// compilation errors and improve build performance. This can be achieved
/// using either traditional include guards (#ifndef/#define/#endif pattern)
/// or the #pragma once directive.
///
/// Example of compliant code with traditional guards:
/// @code
///   // header.h
///   #ifndef HEADER_H
///   #define HEADER_H
///
///   void foo();
///
///   #endif // HEADER_H
/// @endcode
///
/// Example of compliant code with #pragma once:
/// @code
///   // header.h
///   #pragma once
///
///   void foo();
/// @endcode
///
/// Example of non-compliant code (no protection):
/// @code
///   // header.h - WARNING: missing header guard
///   void foo();
/// @endcode
class MissingHeaderGuardCheck : public ClangTidyCheck {
public:
  MissingHeaderGuardCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MISSINGHEADERGUARDCHECK_H
