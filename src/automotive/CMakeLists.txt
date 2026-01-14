set(LLVM_LINK_COMPONENTS
  FrontendOpenMP
  support
  )

function(add_automotive_component name)
  add_library(${name} STATIC ${ARGN})
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

  clangTidyAutomotiveArrayComponent
  clangTidyAutomotiveBitfieldComponent
  clangTidyAutomotiveCommentComponent
  clangTidyAutomotiveExpressionComponent
  clangTidyAutomotiveFunctionComponent
  clangTidyAutomotiveLiteralComponent
  clangTidyAutomotiveOperatorComponent
  clangTidyAutomotivePointerComponent
  clangTidyAutomotivePreprocessorComponent
  clangTidyAutomotiveReturnComponent
  clangTidyAutomotiveStatementComponent
  clangTidyAutomotiveStdlibComponent
  clangTidyAutomotiveStorageComponent
  clangTidyAutomotiveTypeComponent
  clangTidyAutomotiveUnusedCode
  clangTidyAutomotiveUtils

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
