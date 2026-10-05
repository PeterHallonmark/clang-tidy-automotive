
function(automotive_plugin__add_component prefix name)
  #if (AUTOMOTIVE_IN_TREE)
  #  # LLVM in-tree build
  #  add_clang_library(${prefix}${name} STATIC ${ARGN})
  #else()
    # Standalone plugin / out-of-tree build
    add_library(${prefix}${name} STATIC ${ARGN})

    # Required for linking static libraries into shared modules (.so)
    set_target_properties(${prefix}${name} PROPERTIES POSITION_INDEPENDENT_CODE ON)
  #endif()
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
  clang_target_link_libraries(${prefix}Module
    PRIVATE
    clangAnalysis
    clangAST
    clangASTMatchers
    clangBasic
    clangLex
  )
endfunction()



################# 

#function(add_automotive_plugin_component name)
#  # Standalone plugin
#  add_library(${name} STATIC ${ARGN})
#
#  # Required for linking static libraries into shared modules (.so)
#  set_target_properties(${name} PROPERTIES POSITION_INDEPENDENT_CODE ON)
#endfunction()  

#function(add_automotive_plugin_library name)
#  cmake_parse_arguments(ARG "" "" "LINK_LIBS;DEPENDS" ${ARGN})
#  
#  add_library(${name} SHARED ${ARG_UNPARSED_ARGUMENTS})
#
#  target_link_libraries(${name} PRIVATE ${ARG_LINK_LIBS})
#  set_target_properties(${name} PROPERTIES PREFIX "")
#
#  # Remove "Module" from the filename if it exists.
#  string(REPLACE "Module" "" base_name ${name})
#  string(TOLOWER ${base_name} base_name_lower)
#
#  set_target_properties(${name} PROPERTIES OUTPUT_NAME ${base_name_lower})
#  
#  install(TARGETS ${name} LIBRARY DESTINATION bin)
#endfunction()

#function(automotive_plugin_target_link_libraries target)
#  target_link_libraries(${target} ${ARGN})
#endfunction()


