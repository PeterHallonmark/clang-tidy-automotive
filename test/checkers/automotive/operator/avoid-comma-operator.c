// RUN: %check_clang_tidy %s automotive-avoid-comma-operator %t -- -- -std=c90
// RUN: %check_clang_tidy %s automotive-avoid-comma-operator %t -- -- -std=c99
// RUN: %check_clang_tidy %s automotive-avoid-comma-operator %t -- -- -std=c11

void test() {
  int a = (1, 2);                               /* not complaint */
  // CHECK-MESSAGES: [[@LINE]]:13: warning: avoid comma operator [automotive-avoid-comma-operator]
  
  for (int i = 0, j = 1; i < 10; ++i, ++j) {    /* Not complaint */
  // CHECK-MESSAGES: [[@LINE]]:37: warning: avoid comma operator [automotive-avoid-comma-operator]
  }
}

