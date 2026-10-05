# ==============================================================================
# AutomotiveBuild.cmake
#
# 
# ==============================================================================

include(AutomotiveBuildMode)

if (CT_AUTOMOTIVE_PLUGIN)
  include(AutomotiveBuildPlugin)
endif()

#include(AutomotiveBuildStandalone)

function(add_automotive_component name)
  if (CT_AUTOMOTIVE_PLUGIN)
    automotive_plugin__add_component(clangTidyAutomotivePlugin ${name} ${ARGN})
  endif()
endfunction()

function(add_automotive_library name)
  cmake_parse_arguments(
    ARG
    ""
    ""
    "COMPONENTS;DEPENDS;LINK_LIBS"
    ${ARGN}
  )

  if (CT_AUTOMOTIVE_PLUGIN)
    automotive_plugin__add_library(clangTidyAutomotivePlugin ${name} ${ARGN})
  endif()
endfunction()

function(automotive_target_link_libraries name)
  cmake_parse_arguments(
    ARG
    ""
    ""
    "PRIVATE"
    ${ARGN}
  )

  if (CT_AUTOMOTIVE_PLUGIN)
    automotive_plugin__target_link_libraries(clangTidyAutomotivePlugin ${name} ${ARGN})
  endif()
endfunction()
