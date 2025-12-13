#!/usr/bin/env python3
"""
Check ID Migration Script for clang-tidy-automotive

This script helps identify and migrate check IDs to follow the naming convention
defined in NAMING-CONVENTION.md.

Usage:
    python3 scripts/check-id-migration.py [--dry-run] [--apply]

Options:
    --dry-run   Show what changes would be made (default)
    --apply     Actually apply the changes to files
"""

import os
import re
import sys
import json
from pathlib import Path
from typing import Dict, List, Tuple, Optional

# Project root directory
PROJECT_ROOT = Path(__file__).parent.parent

# MISRA rule to check ID mapping
MISRA_RULE_MAPPING = {
    # Statement checks
    "15.1": "automotive-avoid-goto",
    "15.3": "automotive-forward-goto-label",
    "15.5": "automotive-avoid-multiple-return-stmt",
    "15.6": "automotive-missing-compound",
    "15.7": "automotive-missing-else",
    "16.2": "automotive-unstructured-switch",
    "16.3": "automotive-missing-break-in-case",
    "16.4": "automotive-missing-default-in-switch",
    "16.5": "automotive-wrong-default-position-in-switch",
    "16.7": "automotive-avoid-boolean-in-switch",
    "2.6": "automotive-unused-label",

    # Expression checks
    "12.3": "automotive-avoid-comma-operator",
    "13.4": "automotive-avoid-assignment-in-expression",
    "14.3": "automotive-avoid-invariant-control",
    "14.4": "automotive-avoid-non-boolean-condition",

    # Literal checks
    "4.1": "automotive-unterminated-escape-sequence",
    "7.1": "automotive-avoid-octal-number",
    "7.3": "automotive-avoid-lowercase-literal-suffix",

    # Type checks
    "6.1": "automotive-wrong-bitfield-type",
    "6.2": "automotive-avoid-signed-single-bitfield",
    "8.1": "automotive-implicit-int",
    "8.12": "automotive-unique-enum-value",
    "19.2": "automotive-avoid-union",

    # Pointer checks
    "11.9": "automotive-wrong-null-pointer-value",

    # Array checks
    "8.11": "automotive-missing-external-array-size",
    "9.3": "automotive-avoid-partial-array-init",
    "18.7": "automotive-avoid-flexible-array-member",
    "18.8": "automotive-avoid-variable-length-array",

    # Function checks
    "8.2": "automotive-uncomplete-function-prototype",
    "8.3": "automotive-function-declaration-mismatch",
    "8.10": "automotive-missing-static-inline",
    "17.1": "automotive-avoid-stdarg-header",
    "17.3": "automotive-implicit-function-decl",
    "17.4": "automotive-missing-return-void",
    "17.7": "automotive-missing-return-value-handling",
    "17.8": "automotive-avoid-function-parameter-modification",

    # Storage checks
    "8.8": "automotive-missing-static-internal-linkage",
    "8.14": "automotive-avoid-restrict-type",

    # Preprocessor checks
    "2.5": "automotive-unused-macro",
    "20.4": "automotive-avoid-macro-named-as-ckeyword",
    "20.5": "automotive-avoid-undef",
    "20.10": "automotive-avoid-hash-operator",
    "20.11": "automotive-avoid-multiple-hash-operators",

    # Comment checks
    "3.1": "automotive-avoid-comment-within-comment",
    "3.2": "automotive-avoid-line-splicing-within-comment",

    # Standard library checks
    "21.3": "automotive-avoid-stdlib-malloc",
    "21.4": "automotive-avoid-setjmp-header",
    "21.5": "automotive-avoid-signal-header",
    "21.7": "automotive-avoid-ascii-to-number",
    "21.8": "automotive-avoid-stdlib-exit",
    "21.12": "automotive-avoid-stdlib-rand",

    # Unused code checks
    "2.3": "automotive-unused-type",
    "2.4": "automotive-unused-tag",
    "2.8": "automotive-unused-object-definition",
}

# Current IDs that need migration (old -> new)
ID_MIGRATIONS = {
    # Fix typo
    "automotive-implict-int": "automotive-implicit-int",

    # Standardize naming pattern
    "automotive-no-return-void": "automotive-missing-return-void",
    "automotive-static-inline": "automotive-missing-static-inline",

    # Standardize stdlib naming
    "automotive-avoid-stdlib-exit-call": "automotive-avoid-stdlib-exit",
    "automotive-avoid-stdlib-malloc-call": "automotive-avoid-stdlib-malloc",
    "automotive-avoid-stdlib-rand-call": "automotive-avoid-stdlib-rand",
    "automotive-avoid-stdlib-system-call": "automotive-avoid-stdlib-system",

    # Migrate rule-based names to descriptive (optional - can keep rule-based)
    # "automotive-c23-adv-13.4": "automotive-avoid-assignment-in-expression",
    # "automotive-c23-req-14.3": "automotive-avoid-invariant-control",
    # "automotive-c23-req-14.4": "automotive-avoid-non-boolean-condition",
    # "automotive-c23-req-16.5": "automotive-wrong-default-position-in-switch",
    # "automotive-x-req-16.2": "automotive-unstructured-switch",
    # "automotive-x-adv-2.4": "automotive-unused-tag",
    # "automotive-c23-adv-2.8": "automotive-unused-object-definition",
}


def find_check_registrations() -> Dict[str, List[Tuple[str, int]]]:
    """Find all check ID registrations in source files."""
    registrations = {}
    src_dir = PROJECT_ROOT / "src" / "automotive"

    pattern = re.compile(r'"(automotive-[^"]+)"')

    for cpp_file in src_dir.rglob("*.cpp"):
        with open(cpp_file, 'r') as f:
            for line_num, line in enumerate(f, 1):
                for match in pattern.finditer(line):
                    check_id = match.group(1)
                    if check_id not in registrations:
                        registrations[check_id] = []
                    registrations[check_id].append((str(cpp_file), line_num))

    return registrations


def find_test_references() -> Dict[str, List[Tuple[str, int]]]:
    """Find all check ID references in test files."""
    references = {}
    test_dir = PROJECT_ROOT / "test" / "checkers" / "automotive"

    pattern = re.compile(r'automotive-[a-z0-9-]+')

    for test_file in test_dir.rglob("*.c"):
        with open(test_file, 'r') as f:
            for line_num, line in enumerate(f, 1):
                for match in pattern.finditer(line):
                    check_id = match.group(0)
                    if check_id not in references:
                        references[check_id] = []
                    references[check_id].append((str(test_file), line_num))

    return references


def analyze_check_ids():
    """Analyze current check IDs and suggest improvements."""
    registrations = find_check_registrations()
    test_refs = find_test_references()

    print("=" * 70)
    print("CHECK ID ANALYSIS")
    print("=" * 70)

    # Check for IDs that need migration
    print("\n## IDs Requiring Migration\n")
    migrations_needed = []
    for old_id, new_id in ID_MIGRATIONS.items():
        if old_id in registrations:
            migrations_needed.append((old_id, new_id))
            print(f"  {old_id}")
            print(f"    -> {new_id}")
            for file, line in registrations[old_id]:
                print(f"       Found in: {file}:{line}")

    if not migrations_needed:
        print("  No migrations needed!")

    # List rule-based IDs (kept for reference)
    print("\n## Rule-Based IDs (Current)\n")
    rule_based = [id for id in registrations if re.match(r'automotive-[cx]\d*-', id)]
    for check_id in sorted(rule_based):
        print(f"  {check_id}")

    # List descriptive IDs
    print("\n## Descriptive IDs (Current)\n")
    descriptive = [id for id in registrations if not re.match(r'automotive-[cx]\d*-', id)]
    for check_id in sorted(descriptive):
        if check_id != "automotive-module":
            print(f"  {check_id}")

    # Summary
    print("\n" + "=" * 70)
    print("SUMMARY")
    print("=" * 70)
    print(f"Total check IDs: {len(registrations)}")
    print(f"Descriptive IDs: {len(descriptive) - 1}")  # -1 for module
    print(f"Rule-based IDs: {len(rule_based)}")
    print(f"Migrations needed: {len(migrations_needed)}")

    return migrations_needed


def apply_migrations(dry_run: bool = True):
    """Apply ID migrations to source and test files."""
    if dry_run:
        print("\n[DRY RUN] Would apply the following changes:\n")
    else:
        print("\n[APPLYING] Making the following changes:\n")

    files_to_update = []

    # Find all files to update
    for pattern, dirs in [("*.cpp", ["src/automotive"]),
                          ("*.h", ["src/automotive"]),
                          ("*.c", ["test/checkers/automotive"]),
                          ("*.md", ["docs", ""])]:
        for dir_name in dirs:
            search_dir = PROJECT_ROOT / dir_name if dir_name else PROJECT_ROOT
            if search_dir.exists():
                files_to_update.extend(search_dir.rglob(pattern))

    changes_made = 0

    for filepath in files_to_update:
        try:
            with open(filepath, 'r') as f:
                content = f.read()
        except Exception as e:
            continue

        original_content = content

        for old_id, new_id in ID_MIGRATIONS.items():
            if old_id in content:
                content = content.replace(old_id, new_id)
                print(f"  {filepath.relative_to(PROJECT_ROOT)}: {old_id} -> {new_id}")
                changes_made += 1

        if content != original_content and not dry_run:
            with open(filepath, 'w') as f:
                f.write(content)

    print(f"\nTotal changes: {changes_made}")

    if dry_run and changes_made > 0:
        print("\nRun with --apply to make these changes.")


def generate_mapping_json():
    """Generate a JSON mapping file for documentation."""
    registrations = find_check_registrations()

    mapping = {
        "version": "1.0.0",
        "description": "Mapping of clang-tidy-automotive check IDs to MISRA rules",
        "checks": {}
    }

    # Reverse the MISRA mapping
    rule_to_id = {v: k for k, v in MISRA_RULE_MAPPING.items()}

    for check_id in sorted(registrations.keys()):
        if check_id == "automotive-module":
            continue

        check_info = {
            "id": check_id,
            "misra_rule": rule_to_id.get(check_id, "unknown"),
            "locations": [{"file": f, "line": l} for f, l in registrations[check_id]]
        }
        mapping["checks"][check_id] = check_info

    output_path = PROJECT_ROOT / "config" / "check-id-mapping.json"
    output_path.parent.mkdir(exist_ok=True)

    with open(output_path, 'w') as f:
        json.dump(mapping, f, indent=2)

    print(f"Generated mapping file: {output_path}")


def main():
    args = sys.argv[1:]

    if "--apply" in args:
        analyze_check_ids()
        apply_migrations(dry_run=False)
    elif "--generate-mapping" in args:
        generate_mapping_json()
    else:
        analyze_check_ids()
        apply_migrations(dry_run=True)


if __name__ == "__main__":
    main()
