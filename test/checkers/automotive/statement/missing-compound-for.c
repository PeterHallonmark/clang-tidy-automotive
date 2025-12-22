// Test file for: automotive-missing-compound
//
// This file tests the detection of missing compound statements for for statements

// RUN: %check_clang_tidy %s automotive-missing-compound %t -- -- -std=c90
// RUN: %check_clang_tidy %s automotive-missing-compound %t -- -- -std=c99
// RUN: %check_clang_tidy %s automotive-missing-compound %t -- -- -std=c11

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

int f1(int x) {
  int i;

  for (i = 0; i < x; ++i)
    x += i;          /* Not compliant */
  // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: missing compound statement
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

int f2(int x) {
  int i;

  for (i = 0; i < x; ++i)
  {                  
    x += i;          /* Compliant */
  }
  return x;
}