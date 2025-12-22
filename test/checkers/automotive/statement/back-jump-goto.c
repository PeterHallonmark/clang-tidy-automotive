// Test file for: automotive-forward-goto-label
//
// This file tests the rule that goto statements should only jump forward
// to labels. Backward jumps (jumping to a label earlier in the code)
// are considered not compliant and should trigger a warning.

// RUN: %check_clang_tidy %s automotive-forward-goto-label %t -- -- -std=c90
// RUN: %check_clang_tidy %s automotive-forward-goto-label %t -- -- -std=c99
// RUN: %check_clang_tidy %s automotive-forward-goto-label %t -- -- -std=c11

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void f(void) {
Label2:
  goto Label2;    /* Not compliant */
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: goto statement jumps backward to label 'Label2'
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void g(void) {
  goto Label1;    /* Compliant */
Label1:
}
