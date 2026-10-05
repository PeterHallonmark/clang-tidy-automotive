//===--- ClangTidyOptions.h - clang-tidy ------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// This wrapper allows the Automotive clang-tidy plugin to be built both
// in-tree (llvm-project) and out-of-tree using installed clang-tidy headers.

#ifndef CLANG_TIDY_PLUGIN_WRAPPER_CLANGTIDYOPTIONS_H
#define CLANG_TIDY_PLUGIN_WRAPPER_CLANGTIDYOPTIONS_H

  #if CT_AUTOMOTIVE_USE_LOCAL_HEADERS

  #else
    #include <clang-tidy/ClangTidyOptions.h>
  #endif // CT_AUTOMOTIVE_USE_LOCAL_HEADERS

#endif // CLANG_TIDY_PLUGIN_WRAPPER_CLANGTIDYOPTIONS_H
