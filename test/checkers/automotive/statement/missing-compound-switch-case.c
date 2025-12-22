// Test file for: automotive-missing-compound
//
// This file tests the detection of missing compound statements for switch statements

// RUN: %check_clang_tidy %s automotive-missing-compound %t -- -- -std=c90
// RUN: %check_clang_tidy %s automotive-missing-compound %t -- -- -std=c99
// RUN: %check_clang_tidy %s automotive-missing-compound %t -- -- -std=c11

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

int f1(int x) {
  switch (x)         /* Not compliant */
    case 1:
      x += 3;        
  // CHECK-MESSAGES: :[[@LINE-2]]:5: warning: missing compound statement

  return x;
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

int f2(int x) {

  switch (x)
  {                  
  case 1:            /* Compliant */
    x += 1;
    break;
  default:
    x += 2;
    break;
  }
  return x;               
}
