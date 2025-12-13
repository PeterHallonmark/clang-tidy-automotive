// Test file for: automotive-avoid-stdlib-rand
// Related MISRA C:2025 Rule: 21.24
//
// This file tests the detection of rand/srand function usage

// RUN: %check_clang_tidy %s automotive-avoid-stdlib-rand %t

#include <stdlib.h>
#include <time.h>

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_rand_violations(void) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: use of srand
    srand(time(0));

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: use of rand
    int x = rand();

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: use of rand
    int y = rand() % 100;
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

// Using a deterministic sequence
static unsigned int seed = 12345;

unsigned int custom_rand(void) {
    seed = seed * 1103515245 + 12345;
    return (seed / 65536) % 32768;
}

void test_compliant(void) {
    // Custom random implementation
    int x = custom_rand();
    (void)x;
}
