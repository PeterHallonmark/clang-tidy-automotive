# Check Implementation Guide

This guide provides detailed implementation guidance for planned clang-tidy-automotive checks.

## High Priority Checks (Required Rules)

### Rule 10.1-10.8: Essential Type Model

These rules deal with the MISRA "essential type" model for type safety.

**Rule 10.1**: Operands shall not be of an inappropriate essential type
- **Complexity**: High
- **Implementation approach**: Create AST matchers for binary operators and check operand types
- **Key challenges**: Implementing the essential type classification system

**Rule 10.3**: Value of expression shall not be assigned to object with narrowing
- **Complexity**: Medium
- **Implementation approach**: Match assignment expressions and compare type sizes
- **Clang support**: `-Wconversion` provides partial coverage

**Rule 10.4**: Both operands shall have the same essential type category
- **Complexity**: Medium
- **Implementation approach**: Check binary operator operands for type compatibility

**Rule 10.5**: Value shall not be cast to inappropriate essential type
- **Complexity**: Medium
- **Implementation approach**: Match cast expressions and validate type compatibility

```cpp
// Example implementation skeleton for Rule 10.3
class NarrowingConversionCheck : public ClangTidyCheck {
public:
  void registerMatchers(MatchFinder *Finder) override {
    Finder->addMatcher(
        binaryOperator(hasOperatorName("="),
                       hasRHS(expr().bind("rhs")),
                       hasLHS(expr().bind("lhs"))),
        this);
  }

  void check(const MatchFinder::MatchResult &Result) override {
    // Compare bit widths of lhs and rhs types
    // Warn if narrowing would occur
  }
};
```

### Rule 11.2-11.8: Pointer Conversions

**Rule 11.3**: Cast shall not be performed between pointer and integral types
- **Complexity**: Medium
- **Implementation approach**: Match cast expressions involving pointers and integers

**Rule 11.4**: Cast shall not convert pointer to object to different pointer type
- **Complexity**: Medium
- **Implementation approach**: Match C-style and reinterpret casts on pointers

**Rule 11.5**: Cast shall not convert void pointer to pointer to object
- **Complexity**: Medium
- **Implementation approach**: Match casts from void* to typed pointers

```cpp
// Example for Rule 11.3
class PointerIntegralCastCheck : public ClangTidyCheck {
public:
  void registerMatchers(MatchFinder *Finder) override {
    Finder->addMatcher(
        castExpr(
            anyOf(
                hasSourceExpression(hasType(isInteger())),
                hasSourceExpression(hasType(pointerType()))
            )
        ).bind("cast"),
        this);
  }
};
```

### Rule 12.2: Shift Operators

**Rule 12.2**: The right operand of shift shall be non-negative and less than width
- **Complexity**: Medium (static cases), High (runtime analysis)
- **Implementation approach**: Match shift operators, analyze right operand for constant values

```cpp
// Example implementation
class ShiftOperatorCheck : public ClangTidyCheck {
public:
  void registerMatchers(MatchFinder *Finder) override {
    Finder->addMatcher(
        binaryOperator(
            anyOf(hasOperatorName("<<"), hasOperatorName(">>")),
            hasRHS(expr().bind("shift_amount"))
        ).bind("shift"),
        this);
  }

  void check(const MatchFinder::MatchResult &Result) override {
    const auto *Shift = Result.Nodes.getNodeAs<BinaryOperator>("shift");
    const auto *Amount = Result.Nodes.getNodeAs<Expr>("shift_amount");

    // Evaluate if constant, check bounds
    Expr::EvalResult EvalResult;
    if (Amount->EvaluateAsInt(EvalResult, *Result.Context)) {
      int64_t ShiftAmount = EvalResult.Val.getInt().getExtValue();
      // Check bounds based on left operand type width
    }
  }
};
```

### Rule 13.1-13.6: Side Effects

**Rule 13.1**: Initializer lists shall not contain persistent side effects
- **Complexity**: High
- **Implementation approach**: Analyze initializer expressions for side effects

**Rule 13.2**: Value of expression and persistent side effects shall be same sequence point
- **Complexity**: Very High
- **Implementation approach**: Complex control flow analysis required

**Rule 13.5**: Right operand of && or || shall not contain persistent side effects
- **Complexity**: Medium
- **Implementation approach**: Match logical operators, analyze RHS for side effects

```cpp
// Helper to detect side effects
bool hasSideEffects(const Expr *E, ASTContext &Ctx) {
  // Check for:
  // - Assignment operators
  // - Increment/decrement
  // - Function calls with non-const parameters
  // - Volatile accesses
  return E->HasSideEffects(Ctx);
}
```

### Rule 18.1-18.6: Pointer Arithmetic

**Rule 18.1**: Pointer arithmetic shall only be applied to array pointers
- **Complexity**: High
- **Implementation approach**: Track pointer origins, validate array context

**Rule 18.2**: Subtraction between pointers shall only apply to same array
- **Complexity**: High
- **Implementation approach**: Alias analysis required

**Rule 18.3**: Relational operators shall not compare pointers to different arrays
- **Complexity**: High
- **Implementation approach**: Similar to 18.2, requires tracking

### Rule 20.1-20.3: Preprocessor Directives

**Rule 20.1**: #include directives should only be preceded by preprocessor directives
- **Complexity**: Low
- **Implementation approach**: Use PPCallbacks, track line contents before #include

**Rule 20.2**: '', ", or \ characters shall not appear in header file names
- **Complexity**: Low
- **Implementation approach**: PPCallbacks::InclusionDirective, validate filename

**Rule 20.3**: #include shall be followed by <filename> or "filename"
- **Complexity**: Low
- **Implementation approach**: PPCallbacks, validate include syntax

```cpp
class IncludeDirectiveCheck : public ClangTidyCheck {
public:
  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override {
    PP->addPPCallbacks(std::make_unique<IncludeCallbacks>(this, SM));
  }
};

class IncludeCallbacks : public PPCallbacks {
  void InclusionDirective(SourceLocation HashLoc,
                          const Token &IncludeTok,
                          StringRef FileName,
                          bool IsAngled,
                          CharSourceRange FilenameRange,
                          /* ... */) override {
    // Validate filename characters
    // Check preceding content
  }
};
```

### Rule 21.1-21.2: Reserved Identifiers

**Rule 21.1**: #define and #undef shall not be used on reserved identifiers
- **Complexity**: Low
- **Implementation approach**: PPCallbacks, check macro name against reserved patterns

**Rule 21.2**: Reserved identifiers shall not be declared
- **Complexity**: Low
- **Implementation approach**: Match declarations, validate identifier names

```cpp
bool isReservedIdentifier(StringRef Name) {
  // Check for:
  // - Starts with underscore followed by uppercase
  // - Starts with double underscore
  // - Standard library names
  if (Name.size() >= 2) {
    if (Name[0] == '_' && (Name[1] == '_' || isupper(Name[1])))
      return true;
  }
  return false;
}
```

## Medium Priority Checks (Advisory)

### Dir 4.2: Language Subset Selection

Document C language extensions used in the project. This is primarily a documentation check.

### Rule 2.7: Unused Parameters

```cpp
class UnusedParameterCheck : public ClangTidyCheck {
public:
  void registerMatchers(MatchFinder *Finder) override {
    Finder->addMatcher(
        functionDecl(
            isDefinition(),
            hasBody(compoundStmt()),
            forEachDescendant(parmVarDecl().bind("param"))
        ).bind("func"),
        this);
  }

  void check(const MatchFinder::MatchResult &Result) override {
    const auto *Param = Result.Nodes.getNodeAs<ParmVarDecl>("param");
    // Check if parameter is referenced in function body
    if (!Param->isUsed()) {
      diag(Param->getLocation(), "unused parameter '%0'") << Param->getName();
    }
  }
};
```

### Rule 4.2: Trigraph Sequences

Clang warns about trigraphs by default (-Wtrigraphs). Consider wrapping this warning.

### Rule 8.9: Local Object Scope

Objects should be declared at the narrowest possible scope.

### Rule 12.4: Constant Expressions

Expressions used in constant contexts should be constant expressions.

### Rule 15.2: Goto in Same Block

```cpp
class GotoSameBlockCheck : public ClangTidyCheck {
public:
  void registerMatchers(MatchFinder *Finder) override {
    Finder->addMatcher(gotoStmt().bind("goto"), this);
  }

  void check(const MatchFinder::MatchResult &Result) override {
    const auto *Goto = Result.Nodes.getNodeAs<GotoStmt>("goto");
    const LabelDecl *Label = Goto->getLabel();

    // Check if label is in the same block scope as goto
    // This requires analyzing the AST hierarchy
  }
};
```

## Implementation Checklist

When implementing a new check:

1. [ ] Create header file in appropriate component directory
2. [ ] Create implementation file with check logic
3. [ ] Add Doxygen documentation with MISRA rule reference
4. [ ] Register check in component's `*Component.cpp`
5. [ ] Add check to `CMakeLists.txt`
6. [ ] Create test file with violation cases
7. [ ] Create test file with compliant cases
8. [ ] Add example files to `examples/rules/`
9. [ ] Update `docs/MISRA-RULE-INVENTORY.md`
10. [ ] Update `config/misra-rule-mapping.json`
11. [ ] Run existing tests to ensure no regressions

## Testing Approach

### Violation Tests

Test files should trigger diagnostics:

```c
// RUN: %check_clang_tidy %s automotive-check-name %t

void func() {
    // Code that violates the rule
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: expected message
    violation_here;
}
```

### Compliant Tests

Test files should NOT trigger diagnostics:

```c
// RUN: %check_clang_tidy %s automotive-check-name %t

void func() {
    // Code that follows the rule
    compliant_code;
}
// CHECK-MESSAGES: {{^}}$
```

## References

- [Clang AST Matcher Reference](https://clang.llvm.org/docs/LibASTMatchersReference.html)
- [Clang-Tidy Developer Guide](https://clang.llvm.org/extra/clang-tidy/Contributing.html)
- [MISRA C:2025 Guidelines](https://www.misra.org.uk/)
