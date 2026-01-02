// Test file for: automotive-avoid-flexible-array-member
//
// This file tests detection of flexible array members in structure
// definitions. Flexible array members (incomplete array types as the
// last struct member) are prohibited and shall trigger diagnostics
// when used, regardless of whether the structure contains preceding
// members or consists solely of the flexible array.

// RUN: %check_clang_tidy %s automotive-avoid-flexible-array-member %t -- -- -std=c11

#include <stdint.h>

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

struct s {
  uint32_t data[]; // Not compliant
// CHECK-MESSAGES: :[[@LINE-1]]:12: warning: avoid flexible array members
};

struct s2 {
  uint32_t header;
  uint32_t data[]; // Not compliant
// CHECK-MESSAGES: :[[@LINE-1]]:12: warning: avoid flexible array members
};

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

struct t {
  uint32_t data[3];
};

struct t2 {
  uint32_t header;
  uint32_t data[3];
};
