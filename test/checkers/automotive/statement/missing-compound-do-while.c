// RUN: %check_clang_tidy %s automotive-missing-compound %t -- -- -std=c90
// RUN: %check_clang_tidy %s automotive-missing-compound %t -- -- -std=c99
// RUN: %check_clang_tidy %s automotive-missing-compound %t -- -- -std=c11

int f1(int x) {
  do
  {
    x = x + 2;       /* Compliant */
  } while (x < 10);

  do
    x = x + 3;       /* Not compliant */
  // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: missing compound statement [automotive-missing-compound]
  while (x < 20);
}
