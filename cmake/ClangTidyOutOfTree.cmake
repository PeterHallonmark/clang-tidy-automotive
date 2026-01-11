function(add_clang_library name)
  cmake_parse_arguments(ARG "" "" "LINK_LIBS;DEPENDS" ${ARGN})

  add_library(${name} MODULE ${ARG_UNPARSED_ARGUMENTS})

  target_link_libraries(${name}
    PRIVATE
    ${ARG_LINK_LIBS}
  )

  set_target_properties(${name} PROPERTIES
    PREFIX ""
  )

  # Remove "Module" from the filename if it exists.
  string(REPLACE "Module" "" base_name ${name})
  set_target_properties(${name} PROPERTIES OUTPUT_NAME ${base_name})
endfunction()

function(clang_target_link_libraries target)
  target_link_libraries(${target} ${ARGN})
endfunction()
