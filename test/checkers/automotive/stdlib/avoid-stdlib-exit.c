// Test file for: automotive-avoid-stdlib-exit
// Related MISRA C:2025 Rule: 21.8
//
// This file tests the detection of exit/abort function usage

// RUN: %check_clang_tidy %s automotive-avoid-stdlib-exit %t

#include <stdlib.h>

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_exit_violations(void) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: use of exit
    exit(0);
}

void test_exit_failure(void) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: use of exit
    exit(EXIT_FAILURE);
}

void test_abort(void) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: use of abort
    abort();
}

void test_quick_exit(void) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: use of _Exit
    _Exit(1);
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

int main(void) {
    // Returning from main is the proper way to exit
    return 0;
}

// Using return in functions
int check_and_return(int x) {
    if (x < 0) {
        return -1;  // Return error code, don't exit
    }
    return 0;
}
