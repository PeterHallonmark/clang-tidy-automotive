
add_clang_library(clangTidyAutomotiveUnusedCode STATIC
  UnusedCodeModule.cpp
  UnusedObjectDefinitionCheck.cpp
  UnusedParameterCheck.cpp
  UnusedTagCheck.cpp
)
