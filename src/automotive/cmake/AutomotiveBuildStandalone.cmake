
#function(add_automotive_standalone_component name)
#  set(target "${AUTO_NAMESPACE}_${name}")
#
#  # Standalone plugin
#  add_library(${name} STATIC ${ARGN})
#
#  # Required for linking static libraries into shared modules (.so)
#  set_target_properties(${name} PROPERTIES POSITION_INDEPENDENT_CODE ON)
#endfunction()  

#function(add_automotive_standalone_library name)
#  set(target "${AUTO_NAMESPACE}_${name}")
#
#  cmake_parse_arguments(ARG "" "" "LINK_LIBS;DEPENDS" ${ARGN})
#  
#  add_library(${name} SHARED ${ARG_UNPARSED_ARGUMENTS})
#
#  target_link_libraries(${name} PRIVATE ${ARG_LINK_LIBS})
#  set_target_properties(${name} PROPERTIES PREFIX "")
#
#  # Remove "Module" from the filename if it exists.
#  string(REPLACE "Module" "Standalone" base_name ${name})
#  string(TOLOWER ${base_name} base_name_lower)
#
#  set_target_properties(${name} PROPERTIES OUTPUT_NAME ${base_name_lower})
#  
#  install(TARGETS ${name} LIBRARY DESTINATION bin)
#endfunction()

#function(automotive_standalone_target_link_libraries target)
#  target_link_libraries(${target} ${ARGN})
#ndfunction()

#automotive_register_hook(add_component add_automotive_standalone_component)
#automotive_register_hook(add_library add_automotive_standalone_library)
#automotive_register_hook(target_link_libraries automotive_standalone_target_link_libraries)
