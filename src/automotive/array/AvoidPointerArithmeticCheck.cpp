//===--- AvoidPointerArithmeticCheck.cpp - clang-tidy ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidPointerArithmeticCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidPointerArithmeticCheck::registerMatchers(MatchFinder *Finder) {
  // Match binary + and - with pointer operand
  Finder->addMatcher(binaryOperator(hasAnyOperatorName("+", "-", "+=", "-="),
                                    hasEitherOperand(hasType(pointerType())))
                         .bind("binop"),
                     this);

  // Match unary ++ and -- on pointers
  Finder->addMatcher(unaryOperator(hasAnyOperatorName("++", "--"),
                                   hasUnaryOperand(hasType(pointerType())))
                         .bind("unaryop"),
                     this);
}

void AvoidPointerArithmeticCheck::check(
    const MatchFinder::MatchResult &Result) {
  // Skip if in system header
  SourceLocation Loc;
  StringRef OpStr;

  if (const auto *BinOp = Result.Nodes.getNodeAs<BinaryOperator>("binop")) {
    Loc = BinOp->getOperatorLoc();
    OpStr = BinOp->getOpcodeStr();

    // Skip pointer subtraction that results in ptrdiff_t (pointer - pointer)
    // This is actually allowed - we only want to catch pointer +/- integer
    if (BinOp->getOpcode() == BO_Sub &&
        BinOp->getLHS()->getType()->isPointerType() &&
        BinOp->getRHS()->getType()->isPointerType()) {
      return;
    }
  } else if (const auto *UnOp =
                 Result.Nodes.getNodeAs<UnaryOperator>("unaryop")) {
    Loc = UnOp->getOperatorLoc();
    OpStr = UnaryOperator::getOpcodeStr(UnOp->getOpcode());
  } else {
    return;
  }

  if (Loc.isInvalid() || Result.SourceManager->isInSystemHeader(Loc))
    return;

  diag(Loc, "pointer arithmetic using '%0' is error-prone; "
            "consider using array indexing instead")
      << OpStr;
}

} // namespace clang::tidy::automotive
