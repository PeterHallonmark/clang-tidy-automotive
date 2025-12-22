// Test file for: automotive-avoid-goto
//
// This file tests that goto statements should bot be used.

// RUN: %check_clang_tidy %s automotive-avoid-goto %t -- -- -std=c90
// RUN: %check_clang_tidy %s automotive-avoid-goto %t -- -- -std=c99
// RUN: %check_clang_tidy %s automotive-avoid-goto %t -- -- -std=c11

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void f(void) {
  goto Label1;    /* Not compliant */
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: avoid goto statement
Label1:
}

void g(void) {
Label2:
  goto Label2;    /* Not compliant */
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: avoid goto statement
}
