# Automotive Test Index

This document maps test files to their corresponding MISRA rules and check implementations.

## Test Organization

Tests are organized in two ways:
1. **By Component** - Functional category (statement/, expression/, etc.)
2. **By Rule Number** - c2023/ directory for MISRA C:2023 specific tests

## Test File Mapping

### Statement Tests (test/checkers/automotive/statement/)

| Test File | Check ID | MISRA Rule | Description |
|-----------|----------|------------|-------------|
| avoid-goto.c | automotive-avoid-goto | 15.1 | Goto statement detection |
| avoid-goto-edge-cases.c | automotive-avoid-goto | 15.1 | Edge cases for goto detection |
| back-jump-goto.c | automotive-forward-goto-label | 15.3 | Forward jump detection |
| missing-compound-if.c | automotive-missing-compound | 15.6 | Missing braces on if |
| missing-compound-else.c | automotive-missing-compound | 15.6 | Missing braces on else |
| missing-compound-for.c | automotive-missing-compound | 15.6 | Missing braces on for |
| missing-compound-while.c | automotive-missing-compound | 15.6 | Missing braces on while |
| missing-compound-do-while.c | automotive-missing-compound | 15.6 | Missing braces on do-while |
| missing-compound-switch-case.c | automotive-missing-compound | 15.6 | Missing braces on case |
| no-boolean-in-switch.c | automotive-avoid-boolean-in-switch | 16.7 | Boolean in switch |
| unused-label.c | automotive-unused-label | 2.6 | Unused label detection |

### Expression Tests (test/checkers/automotive/expression/)

| Test File | Check ID | MISRA Rule | Description |
|-----------|----------|------------|-------------|
| avoid-assignment-in-expression.c | automotive-c23-adv-13.4 | 13.4 | Assignment as expression |
| avoid-non-boolean-in-condition.c | automotive-c23-req-14.4 | 14.4 | Non-boolean condition |
| expression-edge-cases.c | various | various | Edge case tests |

### Literal Tests (test/checkers/automotive/literal/)

| Test File | Check ID | MISRA Rule | Description |
|-----------|----------|------------|-------------|
| avoid-octal-number.c | automotive-avoid-octal-number | 7.1 | Octal literal detection |
| avoid-octal-edge-cases.c | automotive-avoid-octal-number | 7.1 | Octal edge cases |
| avoid-lowercase-literal-suffix.c | automotive-avoid-lowercase-literal-suffix | 7.3 | Lowercase suffix |
| unterminated-escape-sequence.c | automotive-unterminated-escape-sequence | 4.1 | Escape sequence detection |

### Type Tests (test/checkers/automotive/type/)

| Test File | Check ID | MISRA Rule | Description |
|-----------|----------|------------|-------------|
| avoid-union.c | automotive-avoid-union | 19.2 | Union usage detection |
| implicit-int.c | automotive-implicit-int | 8.1 | Implicit int type |
| unique-enum-value.c | automotive-unique-enum-value | 8.12 | Duplicate enum values |

### Bitfield Tests (test/checkers/automotive/bitfield/)

| Test File | Check ID | MISRA Rule | Description |
|-----------|----------|------------|-------------|
| wrong-bitfield-type.c | automotive-wrong-bitfield-type | 6.1 | Wrong bit-field type |
| avoid-signed-single-bitfield.c | automotive-avoid-signed-single-bitfield | 6.2 | Single-bit signed field |
| avoid-bitfield-in-union.c | automotive-avoid-bitfield-in-union | 19.2 | Bit-field in union |
| bitfield-edge-cases.c | various | various | Edge case tests |

### Function Tests (test/checkers/automotive/function/)

| Test File | Check ID | MISRA Rule | Description |
|-----------|----------|------------|-------------|
| function-parameter-mismatch.c | automotive-function-declaration-mismatch | 8.3 | Declaration mismatch |
| missing-static-inline.c | automotive-missing-static-inline | 8.10 | Static inline detection |
| no-implicit-func-decl.c | automotive-implicit-function-decl | 17.3 | Implicit function decl |
| function-edge-cases.c | various | various | Edge case tests |

### Return Tests (test/checkers/automotive/return/)

| Test File | Check ID | MISRA Rule | Description |
|-----------|----------|------------|-------------|
| missing-return-value-handling.c | automotive-missing-return-value-handling | 17.7 | Ignored return value |
| multiple-return-stmt.c | automotive-avoid-multiple-return-stmt | 15.5 | Multiple returns |
| no-return-void.c | automotive-missing-return-void | 17.4 | Missing return |

### Preprocessor Tests (test/checkers/automotive/preprocessor/)

| Test File | Check ID | MISRA Rule | Description |
|-----------|----------|------------|-------------|
| include-invalid-header.c | various | 20.x | Invalid header check |
| include-valid-header.c | various | 20.x | Valid header check |
| unused-macro.c | automotive-unused-macro | 2.5 | Unused macro detection |
| preprocessor-edge-cases.c | various | various | Edge case tests |

### Standard Library Tests (test/checkers/automotive/stdlib/)

| Test File | Check ID | MISRA Rule | Description |
|-----------|----------|------------|-------------|
| avoid-atox.c | automotive-avoid-ascii-to-number | 21.7 | atoi/atof detection |
| avoid-stdarg-copy.c | automotive-avoid-stdarg-header | 17.1 | stdarg.h with va_copy |
| avoid-stdarg-no-copy.c | automotive-avoid-stdarg-header | 17.1 | stdarg.h without va_copy |
| avoid-stdlib-exit.c | automotive-avoid-stdlib-exit | 21.8 | exit/abort detection |
| avoid-stdlib-malloc.c | automotive-avoid-stdlib-malloc | 21.3 | malloc/free detection |
| avoid-stdlib-rand.c | automotive-avoid-stdlib-rand | 21.12 | rand() detection |

### Pointer Tests (test/checkers/automotive/pointer/)

_Note: Pointer tests should be added here_

### Array Tests (test/checkers/automotive/array/)

| Test File | Check ID | MISRA Rule | Description |
|-----------|----------|------------|-------------|
| avoid-flexible-array-member.c | automotive-avoid-flexible-array-member | 18.7 | Flexible array member |

### Operator Tests (test/checkers/automotive/operator/)

| Test File | Check ID | MISRA Rule | Description |
|-----------|----------|------------|-------------|
| no-comma-operator.c | automotive-avoid-comma-operator | 12.3 | Comma operator |

### C2023 Rule-Specific Tests (test/checkers/automotive/c2023/)

| Directory | Check ID | MISRA Rule | Description |
|-----------|----------|------------|-------------|
| 2.3/ | automotive-unused-type | 2.3 | Unused type declarations |
| 2.4/ | automotive-x-adv-2.4 | 2.4 | Unused tags |
| 2.8/ | automotive-c23-adv-2.8 | 2.8 | Unused object definitions |
| 3.2/ | automotive-avoid-line-splicing-within-comment | 3.2 | Line splicing in comments |
| 8.14/ | automotive-avoid-restrict-type | 8.14 | restrict qualifier |
| 11.9/ | automotive-wrong-null-pointer-value | 11.9 | NULL pointer literal |
| 14.3/ | automotive-c23-req-14.3 | 14.3 | Invariant control expression |
| 17.6/ | automotive-array-static-size | 17.6 | Array static size |
| 21.3/ | automotive-avoid-stdlib-memory | 21.3 | Memory functions |
| 21.5/ | automotive-avoid-signal-header | 21.5 | signal.h usage |

### Negative Tests (test/checkers/automotive/negative/)

These tests verify that compliant code does NOT trigger warnings:

| Test File | Categories Covered |
|-----------|-------------------|
| statement-negative.c | Statement checks (goto, switch, compound) |
| type-negative.c | Type checks (union, enum, implicit) |
| literal-negative.c | Literal checks (octal, suffix, escape) |
| function-negative.c | Function checks (prototype, return) |

## Running Tests

### Run All Automotive Tests
```bash
cd build
ninja check-clang-tools
```

### Run Specific Test File
```bash
./build/bin/clang-tidy \
    --checks="automotive-avoid-goto" \
    test/checkers/automotive/statement/avoid-goto.c --
```

### Run with LIT
```bash
llvm-lit test/checkers/automotive/statement/avoid-goto.c
```

## Adding New Tests

1. Create test file in appropriate component directory
2. Use CHECK-MESSAGES format for expected diagnostics
3. Add entry to this index
4. Include both violation and compliant examples
5. Add edge case tests for boundary conditions
