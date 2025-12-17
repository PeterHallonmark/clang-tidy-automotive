//===--- AvoidCommentWithinCommentCheck.cpp - clang-tidy ------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidCommentWithinCommentCheck.h"
#include "../utils/IgnoreLineSpliceRef.h"
#include "CommentMatch.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

static constexpr std::initializer_list<const char *> DefaultProtocols = {
    "http", "https", "ftp", "ftps", "ssh", "file", "sftp", "svn", "git"};

enum State {
  Normal = 0,
  ExpectCommentStart,
  ExpectCommentEnd,
  ExpectURLPattern,
  CommentSingleLine,
  CommentStart,
  CommentEnd
};

static constexpr State getNextState(State CurrentState, char Ch);

static constexpr State stateNormal(char Ch);
static constexpr State stateExpectCommentStart(char Ch);
static constexpr State stateExpectCommentEnd(char Ch);

void AvoidCommentWithinCommentCheck::registerPPCallbacks(
    const SourceManager &SM, Preprocessor *PP, Preprocessor *ModuleExpanderPP) {
  PP->addCommentHandler(&Handler);
}

AvoidCommentWithinCommentCheck::InternalCommentHandler::InternalCommentHandler(
    AvoidCommentWithinCommentCheck &Check)
    : Check(Check) {
  for (const char *Protocol : DefaultProtocols) {
    Protocols.insert(Protocol);
  }
}

bool AvoidCommentWithinCommentCheck::InternalCommentHandler::HandleComment(
    Preprocessor &PP, SourceRange Comment) {

  StringRef CommentText =
      Lexer::getSourceText(CharSourceRange::getCharRange(Comment),
                           PP.getSourceManager(), PP.getLangOpts());

  CommentMatch Match = matchComment(CommentText);
  if (Match.isValid()) {
    CheckComment(Comment.getBegin().getLocWithOffset(Match.Index),
                 CommentText.substr(Match.Index, Match.Size));
  }

  return false;
}

void AvoidCommentWithinCommentCheck::InternalCommentHandler::CheckComment(
    SourceLocation CommentLoc, StringRef CommentText) {

  IgnoreLineSpliceRef FilteredText(CommentText);
  State CurrentState = State::Normal;
  size_t Index;

  for (auto It = FilteredText.begin(); It != FilteredText.end(); ++It) {
    CurrentState = getNextState(CurrentState, *It);

    switch (CurrentState) {
    case State::CommentSingleLine:
      Index = It.prevPosition();
      Check.diag(CommentLoc.getLocWithOffset(Index),
                 "avoid '//' within comment");
      break;

    case State::CommentStart:
      Index = It.prevPosition();
      Check.diag(CommentLoc.getLocWithOffset(Index),
                 "avoid '/*' within comment");
      break;

    case State::CommentEnd:
      Index = It.prevPosition();
      Check.diag(CommentLoc.getLocWithOffset(Index),
                 "avoid '*/' within comment");
      break;

    case ExpectURLPattern:
      Index = It.position();
      std::advance(It, checkURLProtocol(CommentLoc, CommentText, Index));
      break;

    default:
      break;
    }
  }
}

size_t AvoidCommentWithinCommentCheck::InternalCommentHandler::checkURLProtocol(
    SourceLocation CommentLoc, StringRef CommentText, size_t Index) {
  if (CommentText.substr(Index, 3) != "://")
    return 0;

  // Find the start of the protocol by scanning backwards
  size_t Start = Index;
  while (Start > 0 && llvm::isAlnum(CommentText[Start - 1]))
    --Start;

  StringRef Protocol = CommentText.substr(Start, Index - Start);
  if (!Protocols.contains(Protocol)) {
    Check.diag(CommentLoc.getLocWithOffset(Index),
               "unknown protocol '%0'", DiagnosticIDs::Note)
        << Protocol;
    return 2;  
  }
  return 0;
}

static constexpr State getNextState(State CurrentState, char Ch) {
  switch (CurrentState) {
  case State::ExpectCommentStart:
    return stateExpectCommentStart(Ch);

  case State::ExpectCommentEnd:
    return stateExpectCommentEnd(Ch);

  default:
    return stateNormal(Ch);
  }
}

static constexpr State stateNormal(char Ch) {
  switch (Ch) {
  case '/':
    return State::ExpectCommentStart;

  case '*':
    return State::ExpectCommentEnd;

  case ':':
    return State::ExpectURLPattern;

  default:
    return State::Normal;
  }
}

static constexpr State stateExpectCommentStart(char Ch) {
  switch (Ch) {
  case '/':
    return State::CommentSingleLine;

  case '*':
    return State::CommentStart;

  default:
    return State::Normal;
  }
}

static constexpr State stateExpectCommentEnd(char Ch) {
  switch (Ch) {
  case '/':
    return State::CommentEnd;

  default:
    return State::Normal;
  }
}

} // namespace clang::tidy::automotive
