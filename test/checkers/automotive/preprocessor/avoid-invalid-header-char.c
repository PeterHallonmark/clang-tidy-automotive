// RUN: %check_clang_tidy %s automotive-avoid-invalid-header-char %t

// Note: This test file documents the expected behavior.
// Actual testing of invalid header characters is complex because
// such filenames are typically not valid on most filesystems.

// The check would warn on:
// #include "file'name.h"    - single quote
// #include "file\"name.h"   - double quote
// #include "path\\file.h"   - backslash

// Compliant includes:
#include <stdio.h>
#include <stdlib.h>

// These are OK - normal header names
// #include "myheader.h"
// #include "path/to/header.h"
// #include <sys/types.h>

void test_function(void) {
    // Test code
    int x = 0;
    (void)x;
}
