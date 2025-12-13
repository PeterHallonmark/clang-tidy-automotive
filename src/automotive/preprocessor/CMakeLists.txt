
add_clang_library(clangTidyAutomotivePreprocessorComponent STATIC
  AvoidCodeBeforeIncludeCheck.cpp
  AvoidHashOperatorCheck.cpp
  AvoidInvalidHeaderCharCheck.cpp
  AvoidMacroNamedAsCkeywordCheck.cpp
  AvoidMultipleHashOperatorsCheck.cpp
  AvoidReservedMacroIdentifierCheck.cpp
  AvoidUndefCheck.cpp
  PreprocessorComponent.cpp
  PreprocessorFlowCheck.cpp
  UnusedMacroCheck.cpp
)
