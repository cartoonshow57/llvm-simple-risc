add_llvm_component_library(LLVMCGP1Info
  CGP1TargetInfo.cpp

  LINK_COMPONENTS
  MC
  Support

  ADD_TO_COMPONENT
  CGP1
  )

