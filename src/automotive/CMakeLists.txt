set(LLVM_LINK_COMPONENTS
  FrontendOpenMP
  support
  )

add_subdirectory(array)
add_subdirectory(char-sets-and-lexical-convs)
add_subdirectory(comment)
add_subdirectory(declarations-and-definitions)
add_subdirectory(goto-stmt)
add_subdirectory(operator)
add_subdirectory(literal)
add_subdirectory(pointer-type-conversions)
add_subdirectory(bitfield)
add_subdirectory(preprocessor)
add_subdirectory(return-stmt)
add_subdirectory(switch-stmt)
add_subdirectory(unused-code)
add_subdirectory(utils)
add_subdirectory(stdlib)

add_clang_library(clangTidyAutomotiveModule
  AvoidFunctionParameterModificationCheck.cpp
  AvoidNonBooleanInConditionCheck.cpp
  AvoidAssignmentInExpressionCheck.cpp
  ImplicitFunctionDeclCheck.cpp
  ImplicitIntCheck.cpp
  InvariantControlCheck.cpp
  MissingStaticInternalLinkageCheck.cpp
  AvoidUnionCheck.cpp
  AutomotiveTidyModule.cpp
  MissingCompoundCheck.cpp
  MissingElseCheck.cpp

  LINK_LIBS
  clangTidy
  clangTidyUtils
  clangTidyMiscModule
  clangTidyCERTModule

  clangTidyAutomotiveArrayComponent
  clangTidyAutomotiveBitfieldComponent
  clangTidyAutomotiveCharSetsAndLexicalConvs
  clangTidyAutomotiveCommentComponent
  clangTidyAutomotiveDeclarationsAndDefinitions
  clangTidyAutomotiveGotoStmtComponent
  clangTidyAutomotiveOperatorComponent
  clangTidyAutomotiveLiteralComponent
  clangTidyAutomotivePointerTypeConversions
  clangTidyAutomotivePreprocessorComponent
  clangTidyAutomotiveReturnStmtComponent
  clangTidyAutomotiveStdlibComponent
  clangTidyAutomotiveSwitchStmtComponent
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
