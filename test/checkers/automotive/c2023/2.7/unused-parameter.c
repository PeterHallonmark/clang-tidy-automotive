// RUN: %check_clang_tidy %s automotive-c23-adv-2.7 %t -- -- -std=c90
// RUN: %check_clang_tidy %s automotive-c23-adv-2.7 %t -- -- -std=c99
// RUN: %check_clang_tidy %s automotive-c23-adv-2.7 %t -- -- -std=c11

// MISRA C:2023 Rule 2.7 (Advisory)
// There should be no unused parameters in functions.

// Non-compliant: unused parameter
void func_with_unused(int used, int unused) {
  // CHECK-MESSAGES: :[[@LINE-1]]:32: warning: unused parameter 'unused' [automotive-c23-adv-2.7]
  (void)used;
}

// Non-compliant: multiple unused parameters
void func_multiple_unused(int a, int b, int c) {
  // CHECK-MESSAGES: :[[@LINE-1]]:31: warning: unused parameter 'a' [automotive-c23-adv-2.7]
  // CHECK-MESSAGES: :[[@LINE-2]]:38: warning: unused parameter 'b' [automotive-c23-adv-2.7]
  // CHECK-MESSAGES: :[[@LINE-3]]:45: warning: unused parameter 'c' [automotive-c23-adv-2.7]
}

// Compliant: all parameters used
void func_all_used(int x, int y) {
  int sum = x + y;
  (void)sum;
}

// Compliant: unnamed parameter (explicitly unused)
void callback_handler(int event, void *) {
  (void)event;
}

// Compliant: cast to void to indicate intentionally unused
void intentionally_unused(int used, int unused_but_documented) {
  (void)unused_but_documented;  // Intentionally unused for API compatibility
  (void)used;
}

// Compliant: parameter used in expression
int compute(int base, int multiplier) {
  return base * multiplier;
}

// Non-compliant: parameter only partially used (one unused)
void partial_use(int a, int b, int c) {
  // CHECK-MESSAGES: :[[@LINE-1]]:36: warning: unused parameter 'c' [automotive-c23-adv-2.7]
  int result = a + b;
  (void)result;
}

// Compliant: pointer parameter used
void process_data(int *data, int size) {
  for (int i = 0; i < size; ++i) {
    data[i] = 0;
  }
}

// Non-compliant: pointer parameter unused
void unused_pointer(int *ptr) {
  // CHECK-MESSAGES: :[[@LINE-1]]:25: warning: unused parameter 'ptr' [automotive-c23-adv-2.7]
}
