
add_clang_library(clangTidyAutomotiveStatementComponent STATIC
  AvoidBooleanInSwitchCheck.cpp
  AvoidGotoCheck.cpp
  ForwardGotoLabelCheck.cpp
  MissingBreakInCaseStmtCheck.cpp
  MissingCompoundStmtCheck.cpp
  MissingDefaultInSwitchStmtCheck.cpp
  MissingElseCheck.cpp
  MultipleLoopTerminatorsCheck.cpp
  StatementComponent.cpp
  UnstructuredSwitchStmtCheck.cpp
  UnusedLabelCheck.cpp
  WrongOrderInSwitchStmtCheck.cpp
)
