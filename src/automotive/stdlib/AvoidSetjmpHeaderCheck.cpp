//===--- AvoidSetjmpHeaderCheck.cpp - clang-tidy --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidSetjmpHeaderCheck.h"

namespace clang::tidy::automotive {

static const StringRef AvoidFunctions[] = {"setjmp", "longjmp"};

AvoidSetjmpHeaderCheck::AvoidSetjmpHeaderCheck(StringRef Name,
                                               ClangTidyContext *Context)
    : AvoidApiCheck(Name, Context, "setjmp.h", AvoidFunctions) {}

} // namespace clang::tidy::automotive
