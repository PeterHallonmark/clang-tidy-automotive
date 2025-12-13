// Test file for: automotive-implicit-int
// Related MISRA C:2025 Rule: 8.1
//
// This file tests the detection of implicit int declarations
// Note: Most of these are C90 only and may cause warnings in C99+

// RUN: %check_clang_tidy %s automotive-implicit-int %t -- -- -std=c90

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

// Implicit int in declaration
// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: implicit int
const x = 5;

// Implicit int return type (C90)
// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: implicit int
func1() {
    return 0;
}

// Implicit int with storage class
// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: implicit int
static y = 10;

// Implicit int with unsigned
// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: implicit int
unsigned z = 20;

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

// Explicit types
int explicit_int = 5;
unsigned int explicit_uint = 10;
const int explicit_const = 15;
static int explicit_static = 20;

// Explicit function return type
int func2(void) {
    return 0;
}

// Explicit void return
void func3(void) {
    return;
}
