//===--- AvoidMacroIdentifierConflictCheck.cpp - clang-tidy --------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidMacroIdentifierConflictCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {

class MacroIdentifierPPCallbacks : public PPCallbacks {
public:
  MacroIdentifierPPCallbacks(AvoidMacroIdentifierConflictCheck &Check,
                             const SourceManager &SM)
      : Check(Check), SM(SM) {}

  void MacroDefined(const Token &MacroNameTok,
                    const MacroDirective *MD) override {
    // Skip macros defined in system headers
    if (SM.isInSystemHeader(MacroNameTok.getLocation()))
      return;

    StringRef MacroName = MacroNameTok.getIdentifierInfo()->getName();
    SourceLocation Loc = MacroNameTok.getLocation();
    Check.addMacro(MacroName, Loc);
  }

private:
  AvoidMacroIdentifierConflictCheck &Check;
  const SourceManager &SM;
};

} // anonymous namespace

void AvoidMacroIdentifierConflictCheck::addMacro(StringRef Name,
                                                 SourceLocation Loc) {
  // Store macro name and location for later comparison with AST identifiers
  MacroNames[Name] = Loc;

  // Check if this macro conflicts with an already seen identifier
  auto It = IdentifierNames.find(Name);
  if (It != IdentifierNames.end()) {
    SourceLocation IdentLoc = It->second.first;
    StringRef IdentType = It->second.second;

    // Report the conflict
    diag(Loc, "macro '%0' conflicts with %1 identifier") << Name << IdentType;
    diag(IdentLoc, "%0 '%1' declared here", DiagnosticIDs::Note)
        << IdentType << Name;
  }
}

void AvoidMacroIdentifierConflictCheck::addIdentifier(StringRef Name,
                                                      SourceLocation Loc,
                                                      StringRef Type) {
  // Store identifier name, location, and type for later comparison with macros
  // Only store the first occurrence of each identifier
  if (IdentifierNames.find(Name) == IdentifierNames.end()) {
    IdentifierNames[Name] = std::make_pair(Loc, Type.str());
  }
}

void AvoidMacroIdentifierConflictCheck::registerPPCallbacks(
    const SourceManager &SM, Preprocessor *PP, Preprocessor *ModuleExpanderPP) {
  PP->addPPCallbacks(std::make_unique<MacroIdentifierPPCallbacks>(*this, SM));
}

void AvoidMacroIdentifierConflictCheck::registerMatchers(MatchFinder *Finder) {
  // Match variable declarations
  Finder->addMatcher(varDecl(unless(isImplicit())).bind("var"), this);

  // Match function declarations
  Finder->addMatcher(functionDecl(unless(isImplicit())).bind("func"), this);

  // Match typedef declarations
  Finder->addMatcher(typedefDecl(unless(isImplicit())).bind("typedef"), this);

  // Match type alias declarations (using)
  Finder->addMatcher(typeAliasDecl(unless(isImplicit())).bind("alias"), this);

  // Match record declarations (struct, union)
  Finder->addMatcher(recordDecl(unless(isImplicit())).bind("record"), this);

  // Match enum declarations
  Finder->addMatcher(enumDecl().bind("enum"), this);

  // Match enum constant declarations
  Finder->addMatcher(enumConstantDecl().bind("enumConst"), this);

  // Match label statements
  Finder->addMatcher(labelStmt().bind("label"), this);
}

void AvoidMacroIdentifierConflictCheck::check(
    const MatchFinder::MatchResult &Result) {
  const NamedDecl *Decl = nullptr;
  StringRef DeclType;
  std::string Name;
  SourceLocation DeclLoc;

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
  } else if (const auto *EC =
                 Result.Nodes.getNodeAs<EnumConstantDecl>("enumConst")) {
    Decl = EC;
    DeclType = "enum constant";
  } else if (const auto *LS = Result.Nodes.getNodeAs<LabelStmt>("label")) {
    // Handle label statements separately since they don't have a NamedDecl
    Name = LS->getName();
    DeclLoc = LS->getIdentLoc();
    DeclType = "label";
  }

  if (Decl) {
    // Skip declarations in system headers
    if (Result.SourceManager->isInSystemHeader(Decl->getLocation()))
      return;

    // Skip unnamed declarations
    if (!Decl->getIdentifier())
      return;

    Name = Decl->getName().str();
    DeclLoc = Decl->getLocation();
  }

  if (Name.empty())
    return;

  // Store this identifier for later comparison with macros
  addIdentifier(Name, DeclLoc, DeclType);

  // Check if this identifier conflicts with a macro
  auto It = MacroNames.find(Name);
  if (It != MacroNames.end()) {
    SourceLocation MacroLoc = It->second;

    // Report the conflict
    diag(DeclLoc, "%0 '%1' conflicts with macro identifier")
        << DeclType << Name;
    diag(MacroLoc, "macro '%0' defined here", DiagnosticIDs::Note) << Name;
  }
}

void AvoidMacroIdentifierConflictCheck::onEndOfTranslationUnit() {
  // Clear the maps after processing the translation unit
  MacroNames.clear();
  IdentifierNames.clear();
}

} // namespace clang::tidy::automotive
