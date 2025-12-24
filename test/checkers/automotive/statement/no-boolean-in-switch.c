// RUN: %check_clang_tidy %s automotive-avoid-boolean-in-switch %t -- -- -std=c90
// RUN: %check_clang_tidy %s automotive-avoid-boolean-in-switch %t -- -- -std=c99
// RUN: %check_clang_tidy %s automotive-avoid-boolean-in-switch %t -- -- -std=c11

#include <stdbool.h>

void testSwitchWithBoolean() {
  bool flag = true;

  // Test direct boolean variable
  switch (flag) {  // Not compliant
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: avoid boolean expression in switch statement
    case true:
      break;
    case false:
      break;
  }

  // Test logical NOT operation
  switch (!flag) {  // Not compliant
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: avoid boolean expression in switch statement
    case true:
      break;
    default:
      break;
  }

  // Test comparison operation
  int x = 5;
  switch (x == 0) {  // Not compliant
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: avoid boolean expression in switch statement
    case true:
      break;
    case false:
      break;
  }

  // Test logical OR operation
  int y = 10;
  switch ((x == 5) || (y == 10)) {  // Not compliant
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: avoid boolean expression in switch statement
    case true:
      break;
    case false:
      break;
  }

  // Test logical AND operation
  switch ((x > 0) && (y < 20)) {  // Not compliant
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: avoid boolean expression in switch statement
    case true:
      break;
    case false:
      break;
  }

  // Test parenthesized boolean expression
  switch ((x != 10)) {  // Not compliant
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: avoid boolean expression in switch statement
    case true:
      break;
    case false:
      break;
  }

  // Test ternary operator returning boolean
  switch ((x > 0) ? true : false) {  // Non-compliant
  // : :[[@LINE-1]]:3: warning: avoid boolean expression in switch statement
    case true:
      break;
    case false:
      break;
  }

  // Test explicitly cast boolean to int
  switch ((int)(x == 5)) {  // Compliant (allowed since cast forces integer type)
    case 0:
      break;
    case 1:
      break;
  }

  // Test correct integer switch case
  switch (x) {  // Compliant
    case 5:
      break;
    case 10:
      break;
  }
}
