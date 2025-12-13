
add_clang_library(clangTidyAutomotiveExpressionComponent STATIC
  AvoidAssignmentInExpressionCheck.cpp
  AvoidNonBooleanInConditionCheck.cpp
  AvoidSideEffectInLogicalOperandCheck.cpp
  ExpressionComponent.cpp
  InvariantControlCheck.cpp
)
