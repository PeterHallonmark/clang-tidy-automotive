// Test file for: automotive-c23-req-10.1
// Related MISRA C:2023 Rule: 10.1
//
// This file tests the detection of inappropriate essential type mixing

// RUN: %check_clang_tidy %s automotive-c23-req-10.1 %t

#include <stdbool.h>

//===----------------------------------------------------------------------===//
// Violation Cases - Boolean in Arithmetic (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_boolean_arithmetic_violations(void) {
    bool b1 = true;
    bool b2 = false;
    int x = 5;

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: operand of essential type 'Boolean' is inappropriate for arithmetic operation
    int y1 = b1 + 1;

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: operand of essential type 'Boolean' is inappropriate for arithmetic operation
    int y2 = 10 - b2;

    // CHECK-MESSAGES: :[[@LINE+2]]:13: warning: operand of essential type 'Boolean' is inappropriate for arithmetic operation
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: operand of essential type 'Boolean' is inappropriate for arithmetic operation
    int y3 = b1 * b2;

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: operand of essential type 'Boolean' is inappropriate for arithmetic operation
    int y4 = b1 / 2;

    // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: operand of essential type 'Boolean' is inappropriate for arithmetic operation
    int y5 = x + b1;
}

void test_boolean_unary_violations(void) {
    bool b = true;

    // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: operand of essential type 'Boolean' is inappropriate for unary '+' operation
    int y1 = +b;

    // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: operand of essential type 'Boolean' is inappropriate for unary '-' operation
    int y2 = -b;
}

void test_boolean_compound_assignment_violations(void) {
    int x = 10;
    bool b = true;

    // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: operand of essential type 'Boolean' is inappropriate for arithmetic operation
    x += b;

    // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: operand of essential type 'Boolean' is inappropriate for arithmetic operation
    x -= b;

    // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: operand of essential type 'Boolean' is inappropriate for arithmetic operation
    x *= b;
}

//===----------------------------------------------------------------------===//
// Violation Cases - Boolean in Bitwise (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_boolean_bitwise_violations(void) {
    bool b1 = true;
    bool b2 = false;

    // CHECK-MESSAGES: :[[@LINE+2]]:13: warning: operand of essential type 'Boolean' is inappropriate for bitwise operation
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: operand of essential type 'Boolean' is inappropriate for bitwise operation
    int y1 = b1 & b2;

    // CHECK-MESSAGES: :[[@LINE+2]]:13: warning: operand of essential type 'Boolean' is inappropriate for bitwise operation
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: operand of essential type 'Boolean' is inappropriate for bitwise operation
    int y2 = b1 | b2;

    // CHECK-MESSAGES: :[[@LINE+2]]:13: warning: operand of essential type 'Boolean' is inappropriate for bitwise operation
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: operand of essential type 'Boolean' is inappropriate for bitwise operation
    int y3 = b1 ^ b2;

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: operand of essential type 'Boolean' is inappropriate for bitwise operation
    int y4 = b1 << 2;

    // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: operand of essential type 'Boolean' is inappropriate for bitwise NOT operation
    int y5 = ~b1;
}

//===----------------------------------------------------------------------===//
// Violation Cases - Character in Arithmetic (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_character_arithmetic_violations(void) {
    char c1 = 'A';
    char c2 = 'B';

    // CHECK-MESSAGES: :[[@LINE+2]]:13: warning: operand of essential type 'Character' is inappropriate for arithmetic operation
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: operand of essential type 'Character' is inappropriate for arithmetic operation
    int y1 = c1 + c2;

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: operand of essential type 'Character' is inappropriate for arithmetic operation
    int y2 = c1 * 2;

    // CHECK-MESSAGES: :[[@LINE+2]]:13: warning: operand of essential type 'Character' is inappropriate for arithmetic operation
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: operand of essential type 'Character' is inappropriate for arithmetic operation
    int y3 = c1 - c2;
}

void test_character_unary_violations(void) {
    char c = 'X';

    // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: operand of essential type 'Character' is inappropriate for unary '+' operation
    int y1 = +c;

    // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: operand of essential type 'Character' is inappropriate for unary '-' operation
    int y2 = -c;
}

//===----------------------------------------------------------------------===//
// Violation Cases - Character in Bitwise (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_character_bitwise_violations(void) {
    char c = 'A';

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: operand of essential type 'Character' is inappropriate for bitwise operation
    int y1 = c & 0x0F;

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: operand of essential type 'Character' is inappropriate for bitwise operation
    int y2 = c << 1;

    // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: operand of essential type 'Character' is inappropriate for bitwise NOT operation
    int y3 = ~c;
}

//===----------------------------------------------------------------------===//
// Violation Cases - Floating in Bitwise (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_floating_bitwise_violations(void) {
    float f = 3.14f;

    // Note: Many compilers will reject this at parse time, but we still check
    // Bitwise operations on floating-point types
    // Commented out as they may cause compilation errors in the test framework

    // int y1 = ((int)f) & 0x0F;  // This would be a cast, not a direct violation
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_compliant_arithmetic(void) {
    int i1 = 5;
    int i2 = 10;
    unsigned u1 = 3;
    float f1 = 2.5f;

    // Integer arithmetic is compliant
    int y1 = i1 + i2;
    int y2 = i1 - i2;
    int y3 = i1 * i2;
    int y4 = i1 / i2;
    int y5 = i1 % i2;

    // Unsigned arithmetic is compliant
    unsigned y6 = u1 + 5;

    // Floating-point arithmetic is compliant
    float y7 = f1 + 1.5f;
    float y8 = f1 * 2.0f;
}

void test_compliant_bitwise(void) {
    int i = 42;
    unsigned u = 10;

    // Integer bitwise is compliant
    int y1 = i & 0xFF;
    int y2 = i | 0x0F;
    int y3 = i ^ 0xAA;
    int y4 = i << 2;
    int y5 = i >> 1;
    int y6 = ~i;

    // Unsigned bitwise is compliant
    unsigned y7 = u & 0xFF;
    unsigned y8 = u << 3;
}

void test_compliant_boolean_usage(void) {
    bool b1 = true;
    bool b2 = false;
    int x = 5;

    // Boolean in logical operations is compliant
    bool result = b1 && b2;

    // Boolean in conditionals is compliant
    if (b1) {
        x = 10;
    }

    // Comparison operations produce boolean results - compliant
    bool cmp = (x > 0);
}

void test_compliant_character_usage(void) {
    char c = 'A';
    char buffer[10];

    // Character assignment is compliant
    buffer[0] = c;

    // Character comparison is compliant (produces boolean)
    bool is_uppercase = (c >= 'A' && c <= 'Z');

    // Casting character to int explicitly before arithmetic is a different issue
    // (not covered by this check)
    int ascii_offset = (int)c + 32;
}

enum Color { RED, GREEN, BLUE };

void test_compliant_enum(void) {
    enum Color c1 = RED;
    enum Color c2 = GREEN;

    // Enum in arithmetic operations is generally considered compliant
    // (though other rules may restrict this)
    int diff = c2 - c1;
}
