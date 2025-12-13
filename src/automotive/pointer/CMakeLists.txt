
add_clang_library(clangTidyAutomotivePointerComponent STATIC
  AvoidAtomicVoidPointerCheck.cpp
  AvoidCastRemovingQualifierCheck.cpp
  PointerComponent.cpp
  WrongNullPointerValueCheck.cpp
)
