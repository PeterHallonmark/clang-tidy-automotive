
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(AutomotiveBuild)

set(LLVM_LINK_COMPONENTS
  FrontendOpenMP
  support
  )

add_subdirectory(array)
add_subdirectory(bitfield)
add_subdirectory(comment)
add_subdirectory(expression)
add_subdirectory(function)
add_subdirectory(operator)
add_subdirectory(literal)
add_subdirectory(pointer)
add_subdirectory(preprocessor)
add_subdirectory(return)
add_subdirectory(statement)
add_subdirectory(stdlib)
add_subdirectory(storage)
add_subdirectory(type)
#add_subdirectory(unused-code)
add_subdirectory(utils)

add_automotive_library(Automotive
  COMPONENTS
    ArrayComponent
    BitfieldComponent
    CommentComponent
    ExpressionComponent
    FunctionComponent
    LiteralComponent
    OperatorComponent
    PointerComponent
    PreprocessorComponent
    ReturnComponent
    StatementComponent
    StdlibComponent
    StorageComponent
    TypeComponent
    #UnusedCode
    Utils

  LINK_LIBS
    clangTidy
    clangTidyUtils  

  DEPENDS
    ClangDriverOptions
)

automotive_target_link_libraries(Automotive
    PRIVATE
      clangAnalysis
      clangAST
      clangASTMatchers
      clangBasic
      clangLex
)

#add_custom_target(automotive-plugin DEPENDS clangTidyAutomotiveModule)
