# ==============================================================================
# AutomotiveBuildPlugin.cmake
#
# 
# ==============================================================================

function(automotive_plugin__add_component prefix name)
  # Standalone plugin
  add_library(${prefix}${name} STATIC ${ARGN})

  # Required for linking static libraries into shared modules (.so)
  set_target_properties(${prefix}${name} PROPERTIES POSITION_INDEPENDENT_CODE ON)
endfunction()  


function(automotive_plugin__add_library prefix name)
  cmake_parse_arguments(
    ARG
    ""
    ""
    "COMPONENTS;DEPENDS;LINK_LIBS"
    ${ARGN}
  )

  set(component_libs)

  foreach(component_name IN LISTS ARG_COMPONENTS)
    list(APPEND component_libs
      ${prefix}${component_name}
    )
  endforeach()

  add_clang_library(${prefix}Module
    ${name}TidyModule.cpp

    LINK_LIBS
    ${ARG_LINK_LIBS}
    ${component_libs}

    DEPENDS
    ${ARG_DEPENDS}
  )
endfunction()

function(automotive_plugin__target_link_libraries prefix name)
  cmake_parse_arguments(
    ARG
    ""
    ""
    "PRIVATE"
    ${ARGN}
  )

  clang_target_link_libraries(${prefix}Module
    PRIVATE
    ${ARG_PRIVATE}
  )
endfunction()
