#!/usr/bin/env python3
"""
Scan files for forbidden patterns (e.g. MISRA references).

Outputs:
    path/to/file:line: Warning: <message>: "<matched text>"

Exit codes:
    0 - no issues found
    1 - issues found
"""

from __future__ import annotations

import os
import re
import sys
from pathlib import Path
from typing import Iterable, List, Pattern, Tuple


# =========================
# Configuration
# =========================

# Directories or files to ignore
EXCLUDE_PATHS: List[str] = [
    ".git",
    "build",
    "dist",
    "__pycache__",
]


# Regex rules: (pattern, message)
RULES: List[Tuple[Pattern[str], str]] = [
    (re.compile(r"\bMISRA\b", re.IGNORECASE), "MISRA name detected")
]


def is_excluded(path: Path, exclude_paths: Iterable[str]) -> bool:
    """Check if path should be excluded."""
    return any(part in exclude_paths for part in path.parts)


def scan_file(path: Path) -> int:
    """Scan a single file and return number of violations."""
    violations = 0

    try:
        with path.open("r", encoding="utf-8", errors="ignore") as file:
            for line_number, line in enumerate(file, start=1):
                for pattern, message in RULES:
                    match = pattern.search(line)
                    if match:
                        print(
                            f"{path}:{line_number}: Warning: {message}"
                        )
                        violations += 1

    except OSError as error:
        print(f"Error reading {path}: {error}", file=sys.stderr)

    return violations


def scan_directory(root: Path) -> int:
    """Scan directory recursively."""
    total_violations = 0

    for path in root.rglob("*"):
        if not path.is_file():
            continue

        if is_excluded(path, EXCLUDE_PATHS):
            continue

        total_violations += scan_file(path)

    return total_violations


# =========================
# Entry point
# =========================

def main() -> int:
    """Main entry point."""
    root = Path(".")

    violations = scan_directory(root)

    if violations > 0:
        print(f"\nFound {violations} violation(s).")
        return 1

    print("No issues found.")
    return 0


if __name__ == "__main__":
    sys.exit(main())