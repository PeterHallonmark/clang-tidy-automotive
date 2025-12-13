
add_clang_library(clangTidyAutomotiveExpressionComponent STATIC
  AvoidAssignmentInExpressionCheck.cpp
  AvoidInappropriateEssentialTypeCheck.cpp
  AvoidNonBooleanInConditionCheck.cpp
  AvoidSideEffectInLogicalOperandCheck.cpp
  AvoidSideEffectInSizeofCheck.cpp
  ExpressionComponent.cpp
  InvariantControlCheck.cpp
)
