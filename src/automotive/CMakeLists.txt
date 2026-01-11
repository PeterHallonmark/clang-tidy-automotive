set(LLVM_LINK_COMPONENTS
  FrontendOpenMP
  support
  )

function(add_automotive_component name)
  add_library(${name} OBJECT ${ARGN})
  set_property(TARGET ${name} PROPERTY POSITION_INDEPENDENT_CODE ON)
endfunction()

add_subdirectory(array)
add_subdirectory(comment)
add_subdirectory(expression)
add_subdirectory(function)
add_subdirectory(operator)
add_subdirectory(literal)
add_subdirectory(pointer)
add_subdirectory(bitfield)
add_subdirectory(preprocessor)
add_subdirectory(return)
add_subdirectory(statement)
add_subdirectory(stdlib)
add_subdirectory(storage)
add_subdirectory(type)
add_subdirectory(unused-code)
add_subdirectory(utils)

add_clang_library(clangTidyAutomotiveModule
  AutomotiveTidyModule.cpp

  LINK_LIBS
  clangTidy
  clangTidyUtils
  clangTidyMiscModule
  clangTidyCERTModule

  $<TARGET_OBJECTS:clangTidyAutomotiveArrayComponent>
  $<TARGET_OBJECTS:clangTidyAutomotiveArrayComponent>
  $<TARGET_OBJECTS:clangTidyAutomotiveBitfieldComponent>
  $<TARGET_OBJECTS:clangTidyAutomotiveCommentComponent>
  $<TARGET_OBJECTS:clangTidyAutomotiveExpressionComponent>
  $<TARGET_OBJECTS:clangTidyAutomotiveFunctionComponent>
  $<TARGET_OBJECTS:clangTidyAutomotiveLiteralComponent>
  $<TARGET_OBJECTS:clangTidyAutomotiveOperatorComponent>
  $<TARGET_OBJECTS:clangTidyAutomotivePointerComponent>
  $<TARGET_OBJECTS:clangTidyAutomotivePreprocessorComponent>
  $<TARGET_OBJECTS:clangTidyAutomotiveReturnComponent>
  $<TARGET_OBJECTS:clangTidyAutomotiveStatementComponent>
  $<TARGET_OBJECTS:clangTidyAutomotiveStdlibComponent>
  $<TARGET_OBJECTS:clangTidyAutomotiveStorageComponent>
  $<TARGET_OBJECTS:clangTidyAutomotiveTypeComponent>
  $<TARGET_OBJECTS:clangTidyAutomotiveUnusedCode>
  $<TARGET_OBJECTS:clangTidyAutomotiveUtils>

  DEPENDS
  ClangDriverOptions
  )

clang_target_link_libraries(clangTidyAutomotiveModule
  PRIVATE
  clangAnalysis
  clangAST
  clangASTMatchers
  clangBasic
  clangLex
  )
