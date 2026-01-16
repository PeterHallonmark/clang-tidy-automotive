# ==============================================================================
# ClangTidyOutOfTree.cmake
#
# Provides compatibility helpers for building the Automotive clang-tidy plugin
# as out-of-tree (standalone) builds.
#
# This file provides minimal replacements for selected LLVM/Clang CMake helpers
# to allow the plugin to be built outside of llvm-project.
# ==============================================================================

set(AUTOMOTIVE_OUT_OF_TREE ON)

function(add_clang_library name)
  cmake_parse_arguments(ARG "" "" "LINK_LIBS;DEPENDS" ${ARGN})

  add_library(${name} MODULE ${ARG_UNPARSED_ARGUMENTS})

  target_link_libraries(${name} PRIVATE ${ARG_LINK_LIBS})
  set_target_properties(${name} PROPERTIES PREFIX "")

  # Remove "Module" from the filename if it exists.
  string(REPLACE "Module" "" base_name ${name})
  string(TOLOWER ${base_name} base_name_lower)

  set_target_properties(${name} PROPERTIES OUTPUT_NAME ${base_name_lower})
  
  install(TARGETS ${name} LIBRARY DESTINATION bin)
endfunction()

function(clang_target_link_libraries target)
  target_link_libraries(${target} ${ARGN})
endfunction()
