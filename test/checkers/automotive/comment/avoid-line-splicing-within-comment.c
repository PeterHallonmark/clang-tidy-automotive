// Test file for: automotive-avoid-line-splicing-within-comment
//
// This file tests detection of line splicing within '//' comments.
// Using the line splice character '\' inside a single-line comment
// extends the comment to the following line during preprocessing.
// Such usage is prohibited and shall trigger diagnostics for each
// occurrence of a line splice within a '//' comment.

// RUN: %check_clang_tidy %s automotive-avoid-line-splicing-within-comment %t -- -- -std=c99
// RUN: %check_clang_tidy %s automotive-avoid-line-splicing-within-comment %t -- -- -std=c11

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

// Another comment \
   with a line splice     // Not compliant
// CHECK-MESSAGES: :[[@LINE-2]]:20: warning: avoid line-spliceing within a '//' comment

// Another comment \
   with two \
   line splices           // Not compliant
// CHECK-MESSAGES: :[[@LINE-3]]:20: warning: avoid line-spliceing within a '//' comment
// CHECK-MESSAGES: :[[@LINE-3]]:13: warning: avoid line-spliceing within a '//' comment

/\
/\
/\
printf("chrismas tree");  // Not compliant
// CHECK-MESSAGES: :[[@LINE-4]]:2: warning: avoid line-spliceing within a '//' comment
// CHECK-MESSAGES: :[[@LINE-4]]:2: warning: avoid line-spliceing within a '//' comment
// CHECK-MESSAGES: :[[@LINE-4]]:2: warning: avoid line-spliceing within a '//' comment

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

// A comment

/* \
   block comment may contain a line splice
*/
