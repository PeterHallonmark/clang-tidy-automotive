// RUN: %check_clang_tidy %s automotive-c23-req-13.5 %t -- -- -std=c99
// RUN: %check_clang_tidy %s automotive-c23-req-13.5 %t -- -- -std=c11

// MISRA C:2025 Rule 13.5 (Required)
// The right hand operand of a logical && or || operator shall not contain
// persistent side effects.

int foo(void);
int bar(int x);
int pure_check(int x);

void test_logical_and(int a, int b) {
  int x = 0;

  // Non-compliant: assignment in right operand of &&
  if (a && (x = foo()))
  // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: right operand of '&&' operator contains side effects that may not be evaluated due to short-circuit evaluation [automotive-c23-req-13.5]
    (void)x;

  // Non-compliant: increment in right operand of &&
  if (a && ++x)
  // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: right operand of '&&' operator contains side effects that may not be evaluated due to short-circuit evaluation [automotive-c23-req-13.5]
    (void)x;

  // Non-compliant: function call in right operand of &&
  if (a && foo())
  // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: right operand of '&&' operator contains side effects that may not be evaluated due to short-circuit evaluation [automotive-c23-req-13.5]
    (void)0;

  // Compliant: simple variable in right operand
  if (a && b)
    (void)0;

  // Compliant: constant in right operand
  if (a && 1)
    (void)0;
}

void test_logical_or(int a, int b) {
  int x = 0;

  // Non-compliant: assignment in right operand of ||
  if (a || (x = foo()))
  // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: right operand of '||' operator contains side effects that may not be evaluated due to short-circuit evaluation [automotive-c23-req-13.5]
    (void)x;

  // Non-compliant: decrement in right operand of ||
  if (a || --x)
  // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: right operand of '||' operator contains side effects that may not be evaluated due to short-circuit evaluation [automotive-c23-req-13.5]
    (void)x;

  // Non-compliant: compound assignment in right operand
  if (a || (x += 1))
  // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: right operand of '||' operator contains side effects that may not be evaluated due to short-circuit evaluation [automotive-c23-req-13.5]
    (void)x;

  // Compliant: simple variable in right operand
  if (a || b)
    (void)0;
}

void test_nested(int a, int b, int c) {
  int x = 0;

  // Non-compliant: nested side effect
  if (a && (b || (x = foo())))
  // CHECK-MESSAGES: :[[@LINE-1]]:18: warning: right operand of '||' operator contains side effects that may not be evaluated due to short-circuit evaluation [automotive-c23-req-13.5]
    (void)x;
}

// Side effects in left operand are allowed (always evaluated)
void test_left_operand_ok(int a) {
  int x = 0;

  // Compliant: side effect in LEFT operand is always evaluated, right operand has no side effects
  if ((x = foo()) && a)
    (void)x;  // No warning expected - 'a' has no side effects
}
