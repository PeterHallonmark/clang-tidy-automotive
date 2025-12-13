
add_clang_library(clangTidyAutomotivePointerComponent STATIC
  AvoidAtomicVoidPointerCheck.cpp
  AvoidCastRemovingQualifierCheck.cpp
  AvoidIncompatiblePointerCastCheck.cpp
  PointerComponent.cpp
  WrongNullPointerValueCheck.cpp
)
