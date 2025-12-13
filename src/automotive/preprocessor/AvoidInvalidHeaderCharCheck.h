//===--- AvoidInvalidHeaderCharCheck.h - clang-tidy -------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDINVALIDHEADERCHARCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDINVALIDHEADERCHARCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-preprocessor
/// @brief Detects invalid characters in #include header file names.
///
/// The characters ', ", and \ shall not appear in header file names used
/// with #include directives. These characters can cause portability issues
/// and undefined behavior across different compilers and platforms.
///
/// @par MISRA C:2025 Rule 20.2
/// The ', " and \ characters shall not occur in a header file name.
/// @par Category: Required
///
/// Example:
/// @code
///   #include "file'name.h"    // Warning: ' in header name
///   #include "path\\file.h"   // Warning: \ in header name
///   #include "file\"name.h"   // Warning: " in header name
/// @endcode
class AvoidInvalidHeaderCharCheck : public ClangTidyCheck {
public:
  AvoidInvalidHeaderCharCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDINVALIDHEADERCHARCHECK_H
