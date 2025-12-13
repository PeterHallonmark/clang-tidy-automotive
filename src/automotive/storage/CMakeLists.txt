
add_clang_library(clangTidyAutomotiveStorageComponent STATIC
  AvoidGlobalObjectCheck.cpp
  AvoidReservedIdentifierDeclarationCheck.cpp
  AvoidRestrictTypeCheck.cpp
  MissingStaticInternalLinkageCheck.cpp
  StorageComponent.cpp
)
