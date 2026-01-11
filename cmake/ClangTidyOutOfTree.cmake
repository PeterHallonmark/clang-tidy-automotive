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
endfunction()

function(clang_target_link_libraries target)
  target_link_libraries(${target} ${ARGN})
endfunction()
