// RUN: %check_clang_tidy %s automotive-c23-mand-17.6 %t -- -- -std=c99
// RUN: %check_clang_tidy %s automotive-c23-mand-17.6 %t -- -- -std=c11

// Non-compliant: static in array parameter
void func_static(int arr[static 10]);
// CHECK-MESSAGES: :[[@LINE-1]]:22: warning: array parameter 'arr' uses 'static' keyword which is not permitted [automotive-c23-mand-17.6]

// Non-compliant: static with size expression
void func_static_expr(int arr[static 5 + 5]);
// CHECK-MESSAGES: :[[@LINE-1]]:27: warning: array parameter 'arr' uses 'static' keyword which is not permitted [automotive-c23-mand-17.6]

// Compliant: regular sized array parameter (decays to pointer)
void func_sized(int arr[10]);

// Compliant: unsized array parameter
void func_unsized(int arr[]);

// Compliant: pointer parameter
void func_pointer(int *arr);

// Compliant: const pointer
void func_const_ptr(const int *arr);

// Function definitions to avoid incomplete type warnings
void func_static(int arr[static 10]) {
// CHECK-MESSAGES: :[[@LINE-1]]:22: warning: array parameter 'arr' uses 'static' keyword which is not permitted [automotive-c23-mand-17.6]
  (void)arr;
}

void func_sized(int arr[10]) {
  (void)arr;
}

void func_unsized(int arr[]) {
  (void)arr;
}

void func_pointer(int *arr) {
  (void)arr;
}
