// RUN: %check_clang_tidy %s automotive-c23-req-21.2 %t -- -- -std=c99
// RUN: %check_clang_tidy %s automotive-c23-req-21.2 %t -- -- -std=c11

// MISRA C:2025 Rule 21.2 (Required)
// A reserved identifier or macro name shall not be declared.

// Non-compliant: underscore followed by uppercase
int _Reserved;
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: variable declaration uses reserved identifier '_Reserved' [automotive-c23-req-21.2]

// Non-compliant: double underscore
int __internal;
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: variable declaration uses reserved identifier '__internal' [automotive-c23-req-21.2]

// Non-compliant: reserved function name
void _Init(void);
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: function declaration uses reserved identifier '_Init' [automotive-c23-req-21.2]

// Non-compliant: double underscore in function
void __setup(void);
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: function declaration uses reserved identifier '__setup' [automotive-c23-req-21.2]

// Non-compliant: typedef using standard library name
typedef int size_t;
// CHECK-MESSAGES: :[[@LINE-1]]:13: warning: typedef declaration uses reserved identifier 'size_t' [automotive-c23-req-21.2]

// Non-compliant: struct using reserved name
struct _Data {
// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: struct declaration uses reserved identifier '_Data' [automotive-c23-req-21.2]
  int x;
};

// Non-compliant: enum using reserved name
enum __Status {
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: enum declaration uses reserved identifier '__Status' [automotive-c23-req-21.2]
  OK,
  ERROR
};

// Compliant: normal identifiers
int myVariable;
void myFunction(void);
typedef int my_int_t;
struct MyData { int x; };
enum MyStatus { SUCCESS, FAILURE };

// Compliant: single underscore followed by lowercase
int _lowercase;  // This is reserved at file scope but we only check the pattern
