# ==============================================================================
# AutomotiveBuildCore.cmake
#
# Defines whether the Automotive clang-tidy project is built
# in-tree (as part of llvm-project) or out-of-tree (standalone).
#
# Out-of-tree is the default and intended mode.
# In-tree mode is automatically detected when integrated into llvm-project.
# ==============================================================================

include(AutomotiveHooks)

function(add_automotive_component name)
  #add_automotive_plugin_component(${name} ${ARGN})
  automotive_run_hooks(add_component ${name} ${ARGN})
endfunction()  

function(add_clang_library name)
  #add_automotive_plugin_library(${name} ${ARGN})
  automotive_run_hooks(add_library ${name} ${ARGN})
endfunction()

function(clang_target_link_libraries target)
  #automotive_plugin_target_link_libraries(${target} ${ARGN})
  automotive_run_hooks(target_link_libraries ${target} ${ARGN})
endfunction()
