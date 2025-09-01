add_llvm_component_library(LLVMCGP1Disassembler
  CGP1Disassembler.cpp

  LINK_COMPONENTS
  CodeGenTypes
  CGP1Desc
  CGP1Info
  MC
  MCDisassembler
  Support

  ADD_TO_COMPONENT
  CGP1
  )
