
add_clang_library(clangTidyAutomotiveStorageComponent STATIC
  AvoidGlobalObjectCheck.cpp
  AvoidReservedIdentifierDeclarationCheck.cpp
  AvoidRestrictTypeCheck.cpp
  AvoidUninitializedReadCheck.cpp
  MissingStaticInternalLinkageCheck.cpp
  StorageComponent.cpp
)
