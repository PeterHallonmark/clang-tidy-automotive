// Test file for: automotive-missing-compound
//
// This file tests the detection of missing compound statements for if and else statements

// RUN: %check_clang_tidy %s automotive-missing-compound %t -- -- -std=c90
// RUN: %check_clang_tidy %s automotive-missing-compound %t -- -- -std=c99
// RUN: %check_clang_tidy %s automotive-missing-compound %t -- -- -std=c11

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

int f1(int x) {

  if (x == 3)
    x = x + 3;       /* Not compliant */
  // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: missing compound statement
  return x;
}

int f2(int x)
{
  if (x < 10)
    x = x + 3;       /* Not compliant */
  // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: missing compound statement
  else
    x = x + 4;       /* Not compliant */
  // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: missing compound statement

  if (x < 20)
  {
    x = x + 5;       /* Compliant */
  }
  else
    x = x + 6;       /* Not compliant */
  // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: missing compound statement

  return x;
}

int f3(int x)
{
  if (x < 30)
  {
    x = x + 5;       /* Compliant */
  }
  else if (x < 40)   /* Compliant */
    x = x + 6;       /* Not compliant */
  // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: missing compound statement

  if (x < 40)
  {
    x = x + 5;       /* Compliant */
  }
  else if (x < 50)   /* Compliant */
  {
    x = x + 6;       /* Compliant */
  } else
    x = x + 7;       /* Not compliant */
  // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: missing compound statement

  return x;
}

int f4(int x)
{
  if (x < 10) if (x < 20) { /* Not compliant */
  // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: missing compound statement
    x = x + 10;
  }
  return x;
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

int f5(int x) {
  if (x == 2)
  {
    x = x + 2;       /* Compliant */
  }
}

void f6(int x) {
 if (x < 30)
  {
    x = x + 5;       /* Compliant */
  }
  else if (x < 40)   /* Compliant */
  {
    x = x + 6;       /* Compliant */
  }
}  

int f7(int x) {
  if (x < 2) {
    x = x + 2;       /* Compliant */
  } else {
    x = x + 4;       /* Compliant */
  }
  return x;
}

int f8(int x)
{
  if (x < 60)
  {
    x = x + 5;       /* Compliant */
  }
  else if (x < 70)   /* Compliant */
  {
    x = x + 6;       /* Compliant */
  }
  else
  {
    x = x + 7;       /* Compliant */
  }
  return x;
}
