// Test file for: automotive-avoid-stdlib-malloc
// Related MISRA C:2025 Rule: 21.3
//
// This file tests the detection of dynamic memory allocation functions

// RUN: %check_clang_tidy %s automotive-avoid-stdlib-malloc %t

#include <stdlib.h>

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_malloc_violations(void) {
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: use of malloc
    void *p1 = malloc(100);

    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: use of calloc
    void *p2 = calloc(10, sizeof(int));

    // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: use of realloc
    p1 = realloc(p1, 200);

    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: use of free
    free(p1);
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: use of free
    free(p2);
}

void test_aligned_alloc(void) {
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: use of aligned_alloc
    void *p = aligned_alloc(16, 256);
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: use of free
    free(p);
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

// Static allocation
static int buffer[100];

void test_compliant(void) {
    // Stack allocation
    int local_array[50];

    // Using static buffer
    buffer[0] = 1;
    local_array[0] = 2;
}
