import os
import sys
import shlex
import lit.formats

config.name = "clang-tidy-automotive"
config.test_format = lit.formats.ShTest(True)
config.suffixes = [".c", ".cpp", ".test"]
config.test_source_root = os.path.dirname(__file__)
config.test_exec_root = config.test_source_root



python = shlex.quote(sys.executable)
check_clang_tidy = os.path.join(config.test_source_root,
                                "check_clang_tidy.py")

config.substitutions.append(
    ("%check_clang_tidy", f"{python} {check_clang_tidy}")
)
config.substitutions.append(
    ("%clang_tidy", "clang-tidy")
)

clang_bin_dir = os.path.abspath(
    os.path.join(config.test_source_root, "..", "build", "bin")
)

config.environment["PATH"] = (
    clang_bin_dir
    + os.pathsep
    + os.environ.get("PATH", "")
)
