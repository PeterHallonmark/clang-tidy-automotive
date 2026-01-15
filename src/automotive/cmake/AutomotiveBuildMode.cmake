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

function(add_automotive_component name)
  if (AUTOMOTIVE_IN_TREE)
    # LLVM in-tree build
    add_clang_library(${name} STATIC ${ARGN})
  else()
    # Standalone plugin / out-of-tree build
    add_library(${name} STATIC ${ARGN})

    # Required for linking static libraries into shared modules (.so)
    set_target_properties(${name} PROPERTIES POSITION_INDEPENDENT_CODE ON)
  endif()
endfunction()  
