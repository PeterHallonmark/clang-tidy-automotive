# Diagnostic Mapping Design

## Overview

This design introduces a flexible way to remap clang-tidy diagnostic check names and messages into domain-specific 
policies like MISRA, AUTOSAR, or internal coding standards. It shifts responsibility and control to the user through 
configurable policy layers while retaining compatibility with the underlying clang-tidy infrastructure.

## Key Components

### 1. Mapping Original Check Names
- Each original check name (retrieved from `getCheckName()`) can be mapped to **one or more new check identifiers**.
- It should be possible to **retain the original check name** alongside new mapped names.
- This mapping happens **before** diagnostics are passed to the `ClangTidyDiagnosticConsumer` to intercept Clang
  diagnostics.

### 2. Overriding Diagnostic Messages
- Each mapped check can:
  - **Reuse the original diagnostic message**, or
  - **Replace it with a static, custom message** defined in the configuration file.
- Message overrides preserve original `SourceLocation`.

## Configuration

- The mapping configuration is written in **YAML** format.
- Each entry maps one original check name to a list of new identifiers, each optionally with a custom message.

Mapping is applied only if the target policy check is considered active, based on active checks or command line flag.
The system should support loading multiple YAML configuration files.
