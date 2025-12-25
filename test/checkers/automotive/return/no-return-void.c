// Test file for: automotive-missing-return-void
//
// This file tests the detection of return statements with values in void functions

// RUN: %check_clang_tidy %s automotive-missing-return-void %t

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void bad_function1(void) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: return with value in void function
    return 0;
}

void bad_function2(int x) {
    if (x < 0) {
        // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: return with value in void function
        return -1;
    }
    // Do something
}

void bad_function3(void) {
    int result = 42;
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: return with value in void function
    return result;
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

// Void return in void function
void good_function1(void) {
    return;
}

// No explicit return
void good_function2(void) {
    int x = 5;
    x++;
    // Implicit return
}

// Non-void function returning value
int good_function3(void) {
    return 42;  // OK - function returns int
}

// Early return without value
void good_function4(int x) {
    if (x < 0) {
        return;  // OK - no value
    }
    // Continue processing
}
