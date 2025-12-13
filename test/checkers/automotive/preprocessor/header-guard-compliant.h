// RUN: %check_clang_tidy %s automotive-c23-req-dir-4.10 %t

// This header file has proper include guards - no warning expected
#ifndef HEADER_GUARD_COMPLIANT_H
#define HEADER_GUARD_COMPLIANT_H

void foo(void);
void bar(void);

#endif // HEADER_GUARD_COMPLIANT_H
