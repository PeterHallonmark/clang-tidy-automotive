//===--- AvoidLinesplicingWithinCommentCheck.cpp - clang-tidy ------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidLinesplicingWithinCommentCheck.h"
#include "CommentMatch.h"

namespace clang::tidy::automotive {

bool AvoidLinesplicingWithinCommentCheck::InternalCommentHandler::HandleComment(
    Preprocessor &PP, SourceRange Comment) {
  StringRef CommentText =
      Lexer::getSourceText(CharSourceRange::getCharRange(Comment),
                           PP.getSourceManager(), PP.getLangOpts());

  CommentMatch Match = matchComment(CommentText);
  bool LineSpliceExist = false;

  if (Match.isSingleLine()) {
    SourceLocation StartLoc = Comment.getBegin();

    for (size_t Pos = 0;
         (Pos = CommentText.find("\\\n", Pos)) != StringRef::npos; Pos += 2) {
      SourceLocation LineSpliceLoc = StartLoc.getLocWithOffset(Pos);

      if (!FixitEnabled) {
        Check.diag(LineSpliceLoc, "avoid line-splicing within a '//' comment");
      } else {
        Check.diag(LineSpliceLoc, "avoid line-splicing within a '//' comment")
            << FixItHint::CreateRemoval(SourceRange(
                   LineSpliceLoc, LineSpliceLoc.getLocWithOffset(1)));
      }
      LineSpliceExist = true;
    }

    if (FixitEnabled && LineSpliceExist) {
      SourceLocation EndLoc = Comment.getEnd();

      for (size_t Pos = 0;
           (Pos = CommentText.find("*/", Pos)) != StringRef::npos; Pos += 2) {
        SourceLocation EndBlockCommentLoc = StartLoc.getLocWithOffset(Pos);

        Check.diag(EndBlockCommentLoc, "unexpected end of block comment")
            << FixItHint::CreateRemoval(SourceRange(
                   EndBlockCommentLoc, EndBlockCommentLoc.getLocWithOffset(2)));
      }

      Check.diag(StartLoc, "replace single-line comment with block comment")
          << FixItHint::CreateReplacement(
                 SourceRange(StartLoc, StartLoc.getLocWithOffset(Match.Index)),
                 "/*");
      Check.diag(EndLoc, "replace single-line comment with block comment")
          << FixItHint::CreateInsertion(EndLoc, " */");
    }
  }
  return false;
}

void AvoidLinesplicingWithinCommentCheck::registerPPCallbacks(
    const SourceManager &SM, Preprocessor *PP, Preprocessor *ModuleExpanderPP) {

  PP->addCommentHandler(&Handler);
}

} // namespace clang::tidy::automotive
