
add_clang_library(clangTidyAutomotivePointerComponent STATIC
  AvoidAtomicVoidPointerCheck.cpp
  AvoidCastRemovingQualifierCheck.cpp
  AvoidIncompatiblePointerCastCheck.cpp
  AvoidPointerIntegerCastCheck.cpp
  PointerComponent.cpp
  WrongNullPointerValueCheck.cpp
)
