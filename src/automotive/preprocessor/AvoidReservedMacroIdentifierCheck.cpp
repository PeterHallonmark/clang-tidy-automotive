//===--- AvoidReservedMacroIdentifierCheck.cpp - clang-tidy ---------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidReservedMacroIdentifierCheck.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"
#include "llvm/ADT/StringSet.h"
#include <cctype>

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {

// Standard library reserved names that should not be defined/undefined
static const llvm::StringSet<> ReservedNames = {
    "assert",   "errno",  "math_errhandling", "setjmp",  "va_arg",
    "va_copy",  "va_end", "va_start",         "offsetof", "NULL",
    "true",     "false",  "bool",             "FILE",     "fpos_t",
    "size_t",   "ptrdiff_t", "wchar_t",       "int8_t",   "int16_t",
    "int32_t",  "int64_t",   "uint8_t",       "uint16_t", "uint32_t",
    "uint64_t", "intptr_t",  "uintptr_t",     "CHAR_BIT", "CHAR_MAX",
    "CHAR_MIN", "INT_MAX",   "INT_MIN",       "LONG_MAX", "LONG_MIN",
    "SCHAR_MAX","SCHAR_MIN", "SHRT_MAX",      "SHRT_MIN", "UCHAR_MAX",
    "UINT_MAX", "ULONG_MAX", "USHRT_MAX",     "EOF",      "SEEK_CUR",
    "SEEK_END", "SEEK_SET",  "BUFSIZ",        "FILENAME_MAX", "FOPEN_MAX",
    "stdin",    "stdout",    "stderr",        "EXIT_FAILURE", "EXIT_SUCCESS",
    "RAND_MAX", "MB_CUR_MAX"
};

bool isReservedIdentifier(StringRef Name) {
  if (Name.empty())
    return false;

  // Check for underscore followed by uppercase or another underscore
  if (Name.size() >= 2 && Name[0] == '_') {
    if (std::isupper(static_cast<unsigned char>(Name[1])) || Name[1] == '_')
      return true;
  }

  // Check for known standard library names
  return ReservedNames.contains(Name);
}

class ReservedMacroPPCallbacks : public PPCallbacks {
public:
  ReservedMacroPPCallbacks(ClangTidyCheck &Check, const SourceManager &SM)
      : Check(Check), SM(SM) {}

  void MacroDefined(const Token &MacroNameTok,
                    const MacroDirective *MD) override {
    // Skip macros defined in system headers
    if (SM.isInSystemHeader(MacroNameTok.getLocation()))
      return;

    StringRef MacroName = MacroNameTok.getIdentifierInfo()->getName();
    if (isReservedIdentifier(MacroName)) {
      Check.diag(MacroNameTok.getLocation(),
                 "#define of reserved identifier '%0'")
          << MacroName;
    }
  }

  void MacroUndefined(const Token &MacroNameTok, const MacroDefinition &MD,
                      const MacroDirective *Undef) override {
    // Skip undefs in system headers
    if (Undef && SM.isInSystemHeader(Undef->getLocation()))
      return;

    StringRef MacroName = MacroNameTok.getIdentifierInfo()->getName();
    if (isReservedIdentifier(MacroName)) {
      Check.diag(MacroNameTok.getLocation(),
                 "#undef of reserved identifier '%0'")
          << MacroName;
    }
  }

private:
  ClangTidyCheck &Check;
  const SourceManager &SM;
};

} // anonymous namespace

void AvoidReservedMacroIdentifierCheck::registerPPCallbacks(
    const SourceManager &SM, Preprocessor *PP,
    Preprocessor *ModuleExpanderPP) {
  PP->addPPCallbacks(std::make_unique<ReservedMacroPPCallbacks>(*this, SM));
}

} // namespace clang::tidy::automotive
