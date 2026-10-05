# ==============================================================================
# AutomotiveBuildMode.cmake
#
# Defines whether the Automotive clang-tidy project is built
# in-tree (as part of llvm-project) or out-of-tree (standalone).
#
# Out-of-tree is the default and intended mode.
# In-tree mode is automatically detected when integrated into llvm-project.
# ==============================================================================

if (NOT DEFINED AUTOMOTIVE_OUT_OF_TREE)
  set(AUTOMOTIVE_OUT_OF_TREE OFF)
endif()

if (AUTOMOTIVE_OUT_OF_TREE)
  set(AUTOMOTIVE_IN_TREE OFF)
else()
  set(AUTOMOTIVE_IN_TREE ON)
endif()
