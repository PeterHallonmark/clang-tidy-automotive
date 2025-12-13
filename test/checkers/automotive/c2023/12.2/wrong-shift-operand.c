// RUN: %check_clang_tidy %s automotive-c23-req-12.2 %t -- -- -std=c99
// RUN: %check_clang_tidy %s automotive-c23-req-12.2 %t -- -- -std=c11

// MISRA C:2025 Rule 12.2 (Required)
// The right hand operand of a shift operator shall lie in the range zero to
// one less than the width in bits of the essential type of the left hand
// operand.

#include <stdint.h>

void test_shift_bounds(void) {
  uint32_t x = 1;
  uint8_t y = 1;
  int32_t z = 1;

  // Non-compliant: shift amount equals bit width (32 bits)
  uint32_t a = x << 32;
  // CHECK-MESSAGES: :[[@LINE-1]]:20: warning: shift amount (32) is greater than or equal to the width of the type (32 bits) [automotive-c23-req-12.2]

  // Non-compliant: shift amount exceeds bit width
  uint32_t b = x << 33;
  // CHECK-MESSAGES: :[[@LINE-1]]:20: warning: shift amount (33) is greater than or equal to the width of the type (32 bits) [automotive-c23-req-12.2]

  // Non-compliant: shift amount equals bit width for uint8_t (8 bits)
  uint8_t c = y << 8;
  // CHECK-MESSAGES: :[[@LINE-1]]:19: warning: shift amount (8) is greater than or equal to the width of the type (32 bits) [automotive-c23-req-12.2]

  // Non-compliant: right shift with excessive amount
  uint32_t d = x >> 32;
  // CHECK-MESSAGES: :[[@LINE-1]]:20: warning: shift amount (32) is greater than or equal to the width of the type (32 bits) [automotive-c23-req-12.2]

  // Compliant: shift within bounds
  uint32_t e = x << 31;  // Maximum valid shift for 32-bit type
  uint32_t f = x << 0;   // Minimum valid shift
  uint32_t g = x >> 16;  // Valid right shift

  // Compliant: compound assignment shifts within bounds
  x <<= 16;
  x >>= 8;

  // Non-compliant: compound assignment with excessive shift
  x <<= 32;
  // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: shift amount (32) is greater than or equal to the width of the type (32 bits) [automotive-c23-req-12.2]
}

// Test with different integer types
void test_different_types(void) {
  uint64_t u64 = 1;
  uint16_t u16 = 1;

  // Non-compliant: shift equals 64-bit width
  uint64_t a = u64 << 64;
  // CHECK-MESSAGES: :[[@LINE-1]]:22: warning: shift amount (64) is greater than or equal to the width of the type (64 bits) [automotive-c23-req-12.2]

  // Compliant: shift within 64-bit bounds
  uint64_t b = u64 << 63;
}
