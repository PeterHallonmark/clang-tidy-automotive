// Test file for: automotive-unused-label
//
// This file tests that there is no unused label.

// RUN: %check_clang_tidy %s automotive-unused-label %t -- -- -std=c90
// RUN: %check_clang_tidy %s automotive-unused-label %t -- -- -std=c99
// RUN: %check_clang_tidy %s automotive-unused-label %t -- -- -std=c11

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void f(void) {
Label1:        /* Not compliant */
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: unused label 'Label1'
  (void)0;  // Prevent C23 extension warning about label at end of compound statement
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void g(void) {
  goto Label2;
Label2:        /* Compliant */
  (void)0;  // Prevent C23 extension warning
}
