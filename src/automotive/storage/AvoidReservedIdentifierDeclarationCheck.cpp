//===--- AvoidReservedIdentifierDeclarationCheck.cpp - clang-tidy ---------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidReservedIdentifierDeclarationCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "llvm/ADT/StringSet.h"
#include <cctype>

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {

// Standard library reserved names that should not be declared
static const llvm::StringSet<> ReservedNames = {
    "errno",        "math_errhandling",
    "setjmp",       "va_arg",
    "va_copy",      "va_end",
    "va_start",     "offsetof",
    "NULL",         "true",
    "false",        "bool",
    "FILE",         "fpos_t",
    "size_t",       "ptrdiff_t",
    "wchar_t",      "int8_t",
    "int16_t",      "int32_t",
    "int64_t",      "uint8_t",
    "uint16_t",     "uint32_t",
    "uint64_t",     "intptr_t",
    "uintptr_t",    "intmax_t",
    "uintmax_t",    "div_t",
    "ldiv_t",       "lldiv_t",
    "sig_atomic_t", "time_t",
    "clock_t",      "tm",
    "jmp_buf",      "va_list"};

bool isReservedIdentifier(StringRef Name) {
  if (Name.empty())
    return false;

  // Check for underscore followed by uppercase or another underscore
  // These are reserved for any use
  if (Name.size() >= 2 && Name[0] == '_') {
    if (std::isupper(static_cast<unsigned char>(Name[1])) || Name[1] == '_')
      return true;
  }

  // Check for known standard library names
  return ReservedNames.contains(Name);
}

} // anonymous namespace

void AvoidReservedIdentifierDeclarationCheck::registerMatchers(
    MatchFinder *Finder) {
  // Match variable declarations
  Finder->addMatcher(varDecl(unless(isImplicit())).bind("var"), this);

  // Match function declarations
  Finder->addMatcher(functionDecl(unless(isImplicit())).bind("func"), this);

  // Match typedef declarations
  Finder->addMatcher(typedefDecl(unless(isImplicit())).bind("typedef"), this);

  // Match type alias declarations (using)
  Finder->addMatcher(typeAliasDecl(unless(isImplicit())).bind("alias"), this);

  // Match record declarations (struct, union, enum)
  Finder->addMatcher(recordDecl(unless(isImplicit())).bind("record"), this);

  // Match enum declarations
  Finder->addMatcher(enumDecl().bind("enum"), this);
}

void AvoidReservedIdentifierDeclarationCheck::check(
    const MatchFinder::MatchResult &Result) {
  const NamedDecl *Decl = nullptr;
  StringRef DeclType;

  if (const auto *Var = Result.Nodes.getNodeAs<VarDecl>("var")) {
    Decl = Var;
    DeclType = "variable";
  } else if (const auto *Func = Result.Nodes.getNodeAs<FunctionDecl>("func")) {
    Decl = Func;
    DeclType = "function";
  } else if (const auto *TD = Result.Nodes.getNodeAs<TypedefDecl>("typedef")) {
    Decl = TD;
    DeclType = "typedef";
  } else if (const auto *TA = Result.Nodes.getNodeAs<TypeAliasDecl>("alias")) {
    Decl = TA;
    DeclType = "type alias";
  } else if (const auto *RD = Result.Nodes.getNodeAs<RecordDecl>("record")) {
    Decl = RD;
    DeclType = RD->isUnion() ? "union" : "struct";
  } else if (const auto *ED = Result.Nodes.getNodeAs<EnumDecl>("enum")) {
    Decl = ED;
    DeclType = "enum";
  }

  if (!Decl)
    return;

  // Skip declarations in system headers
  if (Result.SourceManager->isInSystemHeader(Decl->getLocation()))
    return;

  // Skip unnamed declarations
  if (!Decl->getIdentifier())
    return;

  StringRef Name = Decl->getName();
  if (isReservedIdentifier(Name)) {
    diag(Decl->getLocation(), "%0 declaration uses reserved identifier '%1'")
        << DeclType << Name;
  }
}

} // namespace clang::tidy::automotive
