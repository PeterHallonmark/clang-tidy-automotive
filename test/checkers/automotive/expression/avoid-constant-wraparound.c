// RUN: %check_clang_tidy %s automotive-c23-adv-12.4 %t

#include <limits.h>

// Test cases for MISRA C:2023 Rule 12.4
// Evaluation of constant expressions should not lead to unsigned integer wrap-around

// Non-compliant: unsigned subtraction wrap-around
const unsigned int x1 = 0u - 1u;
// CHECK-MESSAGES: :[[@LINE-1]]:25: warning: constant expression evaluation leads to unsigned integer wrap-around [automotive-c23-adv-12.4]

const unsigned int x2 = 5u - 10u;
// CHECK-MESSAGES: :[[@LINE-1]]:28: warning: constant expression evaluation leads to unsigned integer wrap-around [automotive-c23-adv-12.4]

// Non-compliant: unsigned addition wrap-around
const unsigned int y1 = UINT_MAX + 1u;
// CHECK-MESSAGES: :[[@LINE-1]]:34: warning: constant expression evaluation leads to unsigned integer wrap-around [automotive-c23-adv-12.4]

const unsigned int y2 = UINT_MAX + UINT_MAX;
// CHECK-MESSAGES: :[[@LINE-1]]:34: warning: constant expression evaluation leads to unsigned integer wrap-around [automotive-c23-adv-12.4]

// Non-compliant: unary minus on unsigned
const unsigned int z1 = -1u;
// CHECK-MESSAGES: :[[@LINE-1]]:25: warning: constant expression evaluation leads to unsigned integer wrap-around [automotive-c23-adv-12.4]

const unsigned int z2 = -100u;
// CHECK-MESSAGES: :[[@LINE-1]]:25: warning: constant expression evaluation leads to unsigned integer wrap-around [automotive-c23-adv-12.4]

// Compliant: unary minus on zero (no wrap-around)
const unsigned int z3 = -0u;  // OK: result is 0

// Compliant: no wrap-around
const unsigned int a1 = 5u + 10u;        // OK
const unsigned int a2 = 100u - 50u;      // OK
const unsigned int a3 = 10u * 20u;       // OK
const unsigned int a4 = UINT_MAX;        // OK: just assignment
const unsigned int a5 = UINT_MAX - 1u;   // OK: no wrap

// Compliant: signed integers (not checked by this rule)
const int b1 = 0 - 1;  // OK: signed arithmetic

// Non-const variables (not checked - only const/global checked)
void test_function(void) {
    unsigned int local1 = 0u - 1u;  // Not checked: local non-const
    unsigned int local2 = 5u + 10u; // OK
}
