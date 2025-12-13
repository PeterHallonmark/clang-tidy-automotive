// RUN: %check_clang_tidy %s automotive-c23-req-dir-4.10 %t

// This header file is missing include guards
// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: header file is missing include guard protection; consider adding '#ifndef MISSING_HEADER_GUARD_H' / '#define MISSING_HEADER_GUARD_H' at the start and '#endif' at the end, or use '#pragma once' [automotive-c23-req-dir-4.10]

void foo(void);
void bar(void);
