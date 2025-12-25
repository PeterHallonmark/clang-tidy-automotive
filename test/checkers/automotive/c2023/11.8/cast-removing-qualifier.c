// RUN: %check_clang_tidy %s automotive-c23-req-11.8 %t -- -- -std=c99
// RUN: %check_clang_tidy %s automotive-c23-req-11.8 %t -- -- -std=c11

void test_const_removal(void) {
  int x = 10;
  const int *pc = &x;

  // Non-compliant: cast removes const
  int *p = (int *)pc;
  // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: cast removes 'const' qualifier from pointer type [automotive-c23-req-11.8]

  // Compliant: no const removal
  const int *pc2 = (const int *)pc;
}

void test_volatile_removal(void) {
  volatile int v = 20;
  volatile int *pv = &v;

  // Non-compliant: cast removes volatile
  int *p = (int *)pv;
  // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: cast removes 'volatile' qualifier from pointer type [automotive-c23-req-11.8]

  // Compliant: no volatile removal
  volatile int *pv2 = (volatile int *)pv;
}

void test_both_removal(void) {
  const volatile int cv = 30;
  const volatile int *pcv = &cv;

  // Non-compliant: cast removes both const and volatile
  int *p = (int *)pcv;
  // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: cast removes 'const' and 'volatile' qualifiers from pointer type [automotive-c23-req-11.8]

  // Non-compliant: cast removes only const
  volatile int *pv = (volatile int *)pcv;
  // CHECK-MESSAGES: :[[@LINE-1]]:22: warning: cast removes 'const' qualifier from pointer type [automotive-c23-req-11.8]

  // Non-compliant: cast removes only volatile
  const int *pc = (const int *)pcv;
  // CHECK-MESSAGES: :[[@LINE-1]]:19: warning: cast removes 'volatile' qualifier from pointer type [automotive-c23-req-11.8]
}

void test_compliant_casts(void) {
  int x = 40;
  int *p = &x;

  // Compliant: adding const is OK
  const int *pc = (const int *)p;

  // Compliant: adding volatile is OK
  volatile int *pv = (volatile int *)p;

  // Compliant: adding both is OK
  const volatile int *pcv = (const volatile int *)p;

  // Compliant: void pointer casts (different rule)
  void *vp = (void *)p;
  int *p2 = (int *)vp;
}

void test_nested_pointers(void) {
  const int **pp;

  // Non-compliant: removing const from pointed-to type
  int **p = (int **)pp;
  // CHECK-MESSAGES: :[[@LINE-1]]:13: warning: cast removes 'const' qualifier from pointer type [automotive-c23-req-11.8]
}
