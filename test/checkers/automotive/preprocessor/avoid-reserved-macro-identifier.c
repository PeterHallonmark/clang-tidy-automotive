// RUN: %check_clang_tidy %s automotive-avoid-reserved-macro-identifier %t

// Test: Reserved identifiers starting with underscore + uppercase
#define _Reserved 1
// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: #define of reserved identifier '_Reserved'

#define _ANOTHER_RESERVED 2
// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: #define of reserved identifier '_ANOTHER_RESERVED'

// Test: Reserved identifiers starting with double underscore
#define __internal 3
// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: #define of reserved identifier '__internal'

#define __DOUBLE_UNDERSCORE 4
// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: #define of reserved identifier '__DOUBLE_UNDERSCORE'

// Test: Standard library reserved names
#define NULL 0
// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: #define of reserved identifier 'NULL'

#define errno 0
// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: #define of reserved identifier 'errno'

#define EOF (-1)
// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: #define of reserved identifier 'EOF'

// Test: #undef of reserved identifiers
#undef NULL
// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: #undef of reserved identifier 'NULL'

// Compliant: Normal macro names
#define MY_MACRO 100
#define myMacro 200
#define VALID_NAME 300
#define _lowercase 400  // lowercase after underscore is OK

void test_function(void) {
    int x = MY_MACRO;
    int y = myMacro;
    (void)x;
    (void)y;
}
