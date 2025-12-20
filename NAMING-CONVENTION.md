# Naming Convention for Automotive Checks

This document defines the naming conventions for clang-tidy-automotive checks, covering both C++ class names and check IDs used in configuration.

## Check ID Convention

All check IDs use the `automotive-` prefix. 

### Descriptive Names

Descriptive names indicate what the check detects without requiring knowledge of MISRA rule numbers.

**Format:** `automotive-{prefix}-{description}`

**Examples:**
- `automotive-avoid-goto` - Detects goto statements
- `automotive-missing-default-in-switch` - Missing default case
- `automotive-wrong-bitfield-type` - Incorrect bit-field type
- `automotive-unused-label` - Unused label declarations

**Prefixes:**
| Prefix | Usage | Example |
|--------|-------|---------|
| `avoid` | Something that should not be done | `automotive-avoid-goto` |
| `missing` | Something important is absent | `automotive-missing-compound` |
| `wrong` | Violates a specific constraint | `automotive-wrong-null-pointer-value` |
| `unused` | Unused code element | `automotive-unused-macro` |
| `implicit` | Implicit behavior that should be explicit | `automotive-implicit-function-decl` |
| `uncomplete` | Incomplete declaration/definition | `automotive-uncomplete-function-prototype` |

## Class Name Convention

Check classes use descriptive names with standard prefixes. The class name does **not** include MISRA rule IDs.

### Class Name Prefixes

- **Avoid**: Something that should not be done
  _Example_: `AvoidCommentWithinCommentCheck`
- **Missing**: Something important is absent
  _Example_: `MissingDefaultInSwitchStmtCheck`
- **Wrong**: Something is incorrect or violates a constraint
  _Example_: `WrongBitfieldTypeCheck`
- **Unstructured**: Layout or structure issues
  _Example_: `UnstructuredSwitchStmtCheck`
- **Unused**: Unused code elements
  _Example_: `UnusedLabelCheck`

### Class and File Naming

1. Class name: `{Prefix}{Description}Check`
2. Header file: `{Prefix}{Description}Check.h`
3. Implementation: `{Prefix}{Description}Check.cpp`

**Example:**
```cpp
// AvoidGotoCheck.h / AvoidGotoCheck.cpp
class AvoidGotoCheck : public ClangTidyCheck {
  // ...
};
```

## Registration

Register checks in the appropriate component's `*Component.cpp`:

```cpp
CheckFactories.registerCheck<AvoidGotoCheck>("automotive-avoid-goto");
```

## Benefits

* **Clarity in Code**: Developers understand checks without memorizing rule IDs
* **Scalability**: Supports multiple MISRA versions simultaneously

## Adding a New Check

1. Choose the appropriate naming pattern (descriptive or rule-based)
2. Name the class descriptively using a standard prefix
3. Create files: `{ClassName}.h` and `{ClassName}.cpp`
4. Register with the chosen check ID in the component file
5. Add test file in `test/checkers/automotive/`
