
add_clang_library(clangTidyAutomotiveFunctionComponent STATIC
  AvoidFunctionParameterModificationCheck.cpp
  FunctionComponent.cpp
  FunctionDeclarationMismatchCheck.cpp
  ImplicitFunctionDeclCheck.cpp
  StaticInlineCheck.cpp
  UncompleteFunctionPrototypeCheck.cpp
)
