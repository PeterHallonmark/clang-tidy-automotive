// Test file for: automotive-avoid-boolean-in-switch
//
// This file tests that a boolean expression is not used as the controlling
// expression of a switch statement.

// RUN: %check_clang_tidy %s automotive-avoid-boolean-in-switch %t -- -- -std=c90
// RUN: %check_clang_tidy %s automotive-avoid-boolean-in-switch %t -- -- -std=c99
// RUN: %check_clang_tidy %s automotive-avoid-boolean-in-switch %t -- -- -std=c11

#include <stdbool.h>

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void f1() {
  bool flag = true;

  // Test direct boolean variable
  switch (flag) {  // Not compliant
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: avoid boolean expression in switch statement
    case true:
      break;
    case false:
      break;
  }
}

void f2() {
  bool flag = true;

  // Test logical NOT operation
  switch (!flag) {  // Not compliant
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: avoid boolean expression in switch statement
    case true:
      break;
    default:
      break;
  }
}

void f3() {
  int x = 5;

  // Test comparison operation
  switch (x == 0) {  // Not compliant
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: avoid boolean expression in switch statement
    case true:
      break;
    case false:
      break;
  }
}

void f4() {
  int x = 5;
  int y = 10;

  // Test logical OR operation
  switch ((x == 5) || (y == 10)) {  // Not compliant
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: avoid boolean expression in switch statement
    case true:
      break;
    case false:
      break;
  }
}

void f5() {
  int x = 5;
  int y = 10;

  // Test logical AND operation
  switch ((x > 0) && (y < 20)) {  // Not compliant
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: avoid boolean expression in switch statement
    case true:
      break;
    case false:
      break;
  }
}

void f6() {
  int x = 5;
  int y = 10;

  // Test parenthesized boolean expression
  switch ((x != 10)) {  // Not compliant
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: avoid boolean expression in switch statement
    case true:
      break;
    case false:
      break;
  }
}

void f7() {
  int x = 5;
  int y = 10;

  // Test ternary operator returning boolean
  switch ((x > 0) ? (x > 1) : (x < 0)) {  // Not compliant
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: avoid boolean expression in switch statement
    case true:
      break;
    case false:
      break;
  }
}

// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void f8() {
  int x = 5;

  // Test explicitly cast boolean to int
  switch ((int)(x == 5)) {  // Compliant (allowed since cast forces integer type)
    case 0:
      break;
    case 1:
      break;
  }
}

void f9() {
  int x = 5;

  // Test correct integer switch case
  switch (x) {  // Compliant
    case 5:
      break;
    case 10:
      break;
  }
}
