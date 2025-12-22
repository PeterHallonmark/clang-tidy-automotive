// Test file for: automotive-missing-compound
//
// This file tests the detection of missing compound statements for 
// 'while' statements

// RUN: %check_clang_tidy %s automotive-missing-compound %t -- -- -std=c90
// RUN: %check_clang_tidy %s automotive-missing-compound %t -- -- -std=c99
// RUN: %check_clang_tidy %s automotive-missing-compound %t -- -- -std=c11

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

int f1(int x) {

  while (x < 10)
    x++;             /* Not compliant */
  // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: missing compound statement

  return x;
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

int f2(int x) {

  while (x > 0)
  {
    x--;             /* Compliant */
  }

  return x;
}
