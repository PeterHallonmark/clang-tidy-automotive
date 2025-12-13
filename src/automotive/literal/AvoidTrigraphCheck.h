//===--- AvoidTrigraphCheck.h - clang-tidy ----------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDTRIGRAPHCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDTRIGRAPHCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects usage of trigraph sequences in source code.
///
/// Trigraphs are three-character sequences starting with ?? that are replaced
/// by the preprocessor before tokenization. They were introduced in C89 to
/// support systems with limited character sets but are now obsolete and can
/// lead to confusion.
///
/// Related MISRA C:2023 Rule: 4.2 - Trigraph sequences should not be used.
///
/// The following trigraphs are detected:
/// - ??= becomes #
/// - ??( becomes [
/// - ??) becomes ]
/// - ??< becomes {
/// - ??> becomes }
/// - ??/ becomes \
/// - ??' becomes ^
/// - ??! becomes |
/// - ??- becomes ~
///
/// Example:
/// \code
///   int array??(??) = {0};  // Warning: trigraph ??( and ??) used
///   ??=define MAX 100       // Warning: trigraph ??= used
/// \endcode
class AvoidTrigraphCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  AvoidTrigraphCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Registers preprocessor callbacks for trigraph detection.
  /// \param SM The source manager.
  /// \param PP The preprocessor instance.
  /// \param ModuleExpanderPP The module expander preprocessor.
  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDTRIGRAPHCHECK_H
