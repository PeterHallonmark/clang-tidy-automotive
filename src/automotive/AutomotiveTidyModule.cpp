//===--- AutomotiveTidyModule.cpp - clang-tidy ----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "../ClangTidy.h"
#include "../ClangTidyModule.h"
#include "../ClangTidyModuleRegistry.h"
#include "../cert/FloatLoopCounter.h"
#include "../misc/NoRecursionCheck.h"
#include "../misc/UnusedParametersCheck.h"

#include "array/ArrayComponent.h"
#include "bitfield/BitfieldComponent.h"
#include "comment/CommentComponent.h"
#include "expression/ExpressionComponent.h"
#include "function/FunctionComponent.h"
#include "literal/LiteralComponent.h"
#include "operator/OperatorComponent.h"
#include "pointer/PointerComponent.h"
#include "preprocessor/PreprocessorComponent.h"
#include "return/ReturnComponent.h"
#include "statement/StatementComponent.h"
#include "stdlib/StdlibComponent.h"
#include "storage/StorageComponent.h"
#include "type/TypeComponent.h"

#include "unused-code/UnusedCodeModule.h"

// using namespace clang::ast_matchers;

namespace clang::tidy {
namespace automotive {

class AutomotiveModule : public ClangTidyModule {
public:
  void addCheckFactories(ClangTidyCheckFactories &CheckFactories) override {

    ArrayComponent::addCheckFactories(CheckFactories);
    BitfieldComponent::addCheckFactories(CheckFactories);
    CommentComponent::addCheckFactories(CheckFactories);
    ExpressionComponent::addCheckFactories(CheckFactories);
    FunctionComponent::addCheckFactories(CheckFactories);
    LiteralComponent::addCheckFactories(CheckFactories);
    OperatorComponent::addCheckFactories(CheckFactories);
    PointerComponent::addCheckFactories(CheckFactories);
    PreprocessorComponent::addCheckFactories(CheckFactories);
    ReturnComponent::addCheckFactories(CheckFactories);
    StatementComponent::addCheckFactories(CheckFactories);
    StdlibComponent::addCheckFactories(CheckFactories);
    StorageComponent::addCheckFactories(CheckFactories);
    TypeComponent::addCheckFactories(CheckFactories);

    /* C 2023
    ---------------------------------------------------------------- */
    CheckFactories.registerCheck<cert::FloatLoopCounter>(
        "automotive-c23-req-14.1");
    CheckFactories.registerCheck<misc::NoRecursionCheck>(
        "automotive-c23-req-17.2");

    /* C 2012
    ---------------------------------------------------------------- */
    CheckFactories.registerCheck<cert::FloatLoopCounter>(
        "automotive-c12-req-14.1");

    /* 2. Unused code
    ---------------------------------------------------------------- */
    //UnusedCodeModule::addCheckFactories(CheckFactories);
  }
};

} // namespace automotive

// Register the Automotive Module using this statically initialized variable.
static ClangTidyModuleRegistry::Add<automotive::AutomotiveModule>
    X("automotive-module", "Adds automotive lint checks.");

// This anchor is used to force the linker to link in the generated object file
// and thus register the AutomotiveModule.
volatile int AutomotiveModuleAnchorSource = 0;

} // namespace clang::tidy
