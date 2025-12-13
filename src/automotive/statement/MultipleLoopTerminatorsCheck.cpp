//===--- MultipleLoopTerminatorsCheck.cpp - clang-tidy --------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MultipleLoopTerminatorsCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {

/// Visitor to count break and goto statements that terminate a loop
class LoopTerminatorCounter : public RecursiveASTVisitor<LoopTerminatorCounter> {
public:
  explicit LoopTerminatorCounter(const Stmt *TargetLoop)
      : TargetLoop(TargetLoop) {}

  bool VisitBreakStmt(BreakStmt *BS) {
    // Only count breaks that terminate our target loop (not nested loops)
    if (CurrentLoopDepth == 1) {
      TerminatorCount++;
      if (TerminatorCount == 2) {
        SecondTerminatorLoc = BS->getBreakLoc();
      }
    }
    return true;
  }

  bool VisitGotoStmt(GotoStmt *GS) {
    // Gotos could terminate the loop if jumping outside
    // For simplicity, count all gotos within the loop body
    if (CurrentLoopDepth == 1) {
      TerminatorCount++;
      if (TerminatorCount == 2) {
        SecondTerminatorLoc = GS->getGotoLoc();
      }
    }
    return true;
  }

  // Track nested loops - breaks in nested loops don't terminate outer loop
  bool TraverseWhileStmt(WhileStmt *WS) {
    if (WS == TargetLoop) {
      CurrentLoopDepth++;
      bool Result = RecursiveASTVisitor::TraverseWhileStmt(WS);
      CurrentLoopDepth--;
      return Result;
    }
    // Nested loop - increment depth to ignore its breaks
    CurrentLoopDepth++;
    bool Result = RecursiveASTVisitor::TraverseWhileStmt(WS);
    CurrentLoopDepth--;
    return Result;
  }

  bool TraverseDoStmt(DoStmt *DS) {
    if (DS == TargetLoop) {
      CurrentLoopDepth++;
      bool Result = RecursiveASTVisitor::TraverseDoStmt(DS);
      CurrentLoopDepth--;
      return Result;
    }
    CurrentLoopDepth++;
    bool Result = RecursiveASTVisitor::TraverseDoStmt(DS);
    CurrentLoopDepth--;
    return Result;
  }

  bool TraverseForStmt(ForStmt *FS) {
    if (FS == TargetLoop) {
      CurrentLoopDepth++;
      bool Result = RecursiveASTVisitor::TraverseForStmt(FS);
      CurrentLoopDepth--;
      return Result;
    }
    CurrentLoopDepth++;
    bool Result = RecursiveASTVisitor::TraverseForStmt(FS);
    CurrentLoopDepth--;
    return Result;
  }

  // Don't count breaks inside switch statements as loop terminators
  bool TraverseSwitchStmt(SwitchStmt *SS) {
    CurrentLoopDepth++;
    bool Result = RecursiveASTVisitor::TraverseSwitchStmt(SS);
    CurrentLoopDepth--;
    return Result;
  }

  unsigned getTerminatorCount() const { return TerminatorCount; }
  SourceLocation getSecondTerminatorLoc() const { return SecondTerminatorLoc; }

private:
  const Stmt *TargetLoop;
  unsigned TerminatorCount = 0;
  unsigned CurrentLoopDepth = 0;
  SourceLocation SecondTerminatorLoc;
};

} // anonymous namespace

void MultipleLoopTerminatorsCheck::registerMatchers(MatchFinder *Finder) {
  // Match while, do-while, and for loops
  Finder->addMatcher(whileStmt().bind("loop"), this);
  Finder->addMatcher(doStmt().bind("loop"), this);
  Finder->addMatcher(forStmt().bind("loop"), this);
}

void MultipleLoopTerminatorsCheck::check(
    const MatchFinder::MatchResult &Result) {
  const Stmt *Loop = Result.Nodes.getNodeAs<Stmt>("loop");
  if (!Loop)
    return;

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(Loop->getBeginLoc()))
    return;

  // Count terminators in this loop
  LoopTerminatorCounter Counter(Loop);
  Counter.TraverseStmt(const_cast<Stmt *>(Loop));

  if (Counter.getTerminatorCount() > 1) {
    diag(Counter.getSecondTerminatorLoc(),
         "loop has more than one break or goto terminator; "
         "only one terminator should be used per loop");
  }
}

} // namespace clang::tidy::automotive
