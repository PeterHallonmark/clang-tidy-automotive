//===--- AvoidCommentWithinCommentCheck.h - clang-tidy ----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDCOMMENTWITHINCOMMENTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDCOMMENTWITHINCOMMENTCHECK_H

#include "../../ClangTidyCheck.h"
#include "clang/Lex/Preprocessor.h"
#include "llvm/ADT/StringSet.h"

namespace clang::tidy::automotive {

/// FIXME: Write a short description.
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misra/comment-within-comment.html
class AvoidCommentWithinCommentCheck : public ClangTidyCheck {
public:
  AvoidCommentWithinCommentCheck(StringRef Name, ClangTidyContext *Context,
                                 int x = 0)
      : ClangTidyCheck(Name, Context), Handler(*this) {}

  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;

private:
  class InternalCommentHandler : public CommentHandler {
  public:
    InternalCommentHandler(AvoidCommentWithinCommentCheck &Check);
    virtual bool HandleComment(Preprocessor &PP, SourceRange Comment) override;

  private:
    void CheckComment(SourceLocation CommentLoc, StringRef CommentText);
    void checkURLProtocol(SourceLocation CommentLoc, StringRef CommentText,
                          size_t Index);

    AvoidCommentWithinCommentCheck &Check;
    llvm::StringSet<> Protocols;
  };

  InternalCommentHandler Handler;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDCOMMENTWITHINCOMMENTCHECK_H
