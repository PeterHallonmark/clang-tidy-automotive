//===--- ClangTidy.h - clang-tidy -------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// This wrapper allows the Automotive clang-tidy plugin to be built both
// in-tree (llvm-project) and out-of-tree using installed clang-tidy headers.

#ifndef CLANG_TIDY_PLUGIN_WRAPPER_CLANGTIDY_H
#define CLANG_TIDY_PLUGIN_WRAPPER_CLANGTIDY_H

  //#ifdef CLANG_TIDY_AUTOMOTIVE_PLUGIN
    #include <clang-tidy/ClangTidy.h>
  //#else
    //#include "../clang-tidy/v20/ClangTidy.h"
  //#endif // CLANG_TIDY_AUTOMOTIVE_PLUGIN

#endif // CLANG_TIDY_PLUGIN_WRAPPER_CLANGTIDY_H
