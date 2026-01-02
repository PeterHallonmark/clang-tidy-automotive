// Test file for: automotive-avoid-comma-operator
//
// This file verifies that use of the comma operator is diagnosed as
// not compliant in expression contexts, including assignments and
// loop control expressions.

// RUN: %check_clang_tidy %s automotive-avoid-comma-operator %t -- -- -std=c90
// RUN: %check_clang_tidy %s automotive-avoid-comma-operator %t -- -- -std=c99
// RUN: %check_clang_tidy %s automotive-avoid-comma-operator %t -- -- -std=c11

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

#define X (1, 2)

int y = X;   /* Not compliant */
  // CHECK-MESSAGES: [[@LINE-1]]:9: warning: avoid comma operator

void test() {
  int a = (1, 2);                               /* not complaint */
  // CHECK-MESSAGES: [[@LINE-1]]:13: warning: avoid comma operator
  
  for (int i = 0, j = 1; i < 10; ++i, ++j) {    /* Not complaint */
  // CHECK-MESSAGES: [[@LINE-1]]:37: warning: avoid comma operator
  }
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void f(int a, int b);

void test2(void) {
  f(1, 2);   /* Compliant: NOT the comma operator */
}

int arr[] = { 1, 2, 3 }; /* Compliant: NOT the comma operator */
