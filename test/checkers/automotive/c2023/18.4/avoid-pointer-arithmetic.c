// RUN: %check_clang_tidy %s automotive-c23-adv-18.4 %t -- -- -std=c99
// RUN: %check_clang_tidy %s automotive-c23-adv-18.4 %t -- -- -std=c11

void test_pointer_addition(void) {
  int arr[10];
  int *p = arr;

  // Non-compliant: pointer + integer
  int *q = p + 3;
  // CHECK-MESSAGES: :[[@LINE-1]]:14: warning: pointer arithmetic using '+' is error-prone; consider using array indexing instead [automotive-c23-adv-18.4]

  // Non-compliant: integer + pointer
  int *r = 2 + p;
  // CHECK-MESSAGES: :[[@LINE-1]]:14: warning: pointer arithmetic using '+' is error-prone; consider using array indexing instead [automotive-c23-adv-18.4]
}

void test_pointer_subtraction(void) {
  int arr[10];
  int *p = arr;

  // Non-compliant: pointer - integer
  int *q = p - 1;
  // CHECK-MESSAGES: :[[@LINE-1]]:14: warning: pointer arithmetic using '-' is error-prone; consider using array indexing instead [automotive-c23-adv-18.4]

  // Compliant: pointer - pointer (yields ptrdiff_t, allowed)
  int *r = &arr[5];
  long diff = r - p;  // No warning - this is allowed
}

void test_compound_assignment(void) {
  int arr[10];
  int *p = arr;

  // Non-compliant: += on pointer
  p += 2;
  // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: pointer arithmetic using '+=' is error-prone; consider using array indexing instead [automotive-c23-adv-18.4]

  // Non-compliant: -= on pointer
  p -= 1;
  // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: pointer arithmetic using '-=' is error-prone; consider using array indexing instead [automotive-c23-adv-18.4]
}

void test_increment_decrement(void) {
  int arr[10];
  int *p = arr;

  // Non-compliant: prefix increment
  ++p;
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: pointer arithmetic using '++' is error-prone; consider using array indexing instead [automotive-c23-adv-18.4]

  // Non-compliant: postfix increment
  p++;
  // CHECK-MESSAGES: :[[@LINE-1]]:4: warning: pointer arithmetic using '++' is error-prone; consider using array indexing instead [automotive-c23-adv-18.4]

  // Non-compliant: prefix decrement
  --p;
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: pointer arithmetic using '--' is error-prone; consider using array indexing instead [automotive-c23-adv-18.4]

  // Non-compliant: postfix decrement
  p--;
  // CHECK-MESSAGES: :[[@LINE-1]]:4: warning: pointer arithmetic using '--' is error-prone; consider using array indexing instead [automotive-c23-adv-18.4]
}

void test_compliant_array_indexing(void) {
  int arr[10];

  // Compliant: array indexing is preferred
  int a = arr[0];
  int b = arr[5];
  arr[3] = 42;

  // Compliant: integer arithmetic (not pointer)
  int x = 5;
  x++;
  x += 2;
  int y = x + 3;
}
