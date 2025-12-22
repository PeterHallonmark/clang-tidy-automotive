// Test file for: automotive-missing-compound
//
// This file tests the detection of missing compound statements for 
// 'do-while' statements

// RUN: %check_clang_tidy %s automotive-missing-compound %t -- -- -std=c90
// RUN: %check_clang_tidy %s automotive-missing-compound %t -- -- -std=c99
// RUN: %check_clang_tidy %s automotive-missing-compound %t -- -- -std=c11

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

int f1(int x) {
  do
    x = x + 3;       /* Not compliant */
  // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: missing compound statement [automotive-missing-compound]
  while (x < 20);
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

int f2(int x) {
  do
  {
    x = x + 2;       /* Compliant */
  } while (x < 10);
}
