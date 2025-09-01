include_directories( ${CMAKE_CURRENT_BINARY_DIR}/.. ${CMAKE_CURRENT_SOURCE_DIR}/.. )

add_llvm_component_library(LLVMCGP1AsmParser
  CGP1AsmParser.cpp

  LINK_COMPONENTS
  CodeGenTypes
  CGP1Desc
  CGP1Info
  MC
  MCParser
  Support

  ADD_TO_COMPONENT
  CGP1
  )

add_dependencies( LLVMCGP1AsmParser CGP1CommonTableGen )
