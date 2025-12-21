// RUN: %check_clang_tidy %s automotive-avoid-line-splicing-within-comment %t -- -- -std=c99
// RUN: %check_clang_tidy %s automotive-avoid-line-splicing-within-comment %t -- -- -std=c11

// A comment              Compliant

// Another comment \
   with a line splice     Not compliant
// CHECK-MESSAGES: :[[@LINE-2]]:20: warning: avoid line-spliceing within a '//' comment [automotive-avoid-line-splicing-within-comment]

// Another comment \
   with two \
   line splices           Not compliant
// CHECK-MESSAGES: :[[@LINE-3]]:20: warning: avoid line-spliceing within a '//' comment [automotive-avoid-line-splicing-within-comment]
// CHECK-MESSAGES: :[[@LINE-3]]:13: warning: avoid line-spliceing within a '//' comment [automotive-avoid-line-splicing-within-comment]

/\
/\
/\
printf("chrismas tree");  Not compliant 
// CHECK-MESSAGES: :[[@LINE-4]]:2: warning: avoid line-spliceing within a '//' comment [automotive-avoid-line-splicing-within-comment]
// CHECK-MESSAGES: :[[@LINE-4]]:2: warning: avoid line-spliceing within a '//' comment [automotive-avoid-line-splicing-within-comment]
// CHECK-MESSAGES: :[[@LINE-4]]:2: warning: avoid line-spliceing within a '//' comment [automotive-avoid-line-splicing-within-comment]

/* \
                          Compliant - block comment '/*' allows line splice
*/
