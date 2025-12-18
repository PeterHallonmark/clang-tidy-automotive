// RUN: %check_clang_tidy %s automotive-avoid-flexible-array-member %t -- -- -std=c11

#include <stdint.h>

struct s {
  uint32_t data[]; // Not compliant
// CHECK-MESSAGES: :[[@LINE-1]]:12: warning: avoid flexible array members [automotive-avoid-flexible-array-member]
};

struct t {
  uint32_t data[3];
};
