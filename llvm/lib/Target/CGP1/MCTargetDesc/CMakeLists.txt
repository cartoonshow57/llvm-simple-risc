add_llvm_component_library(LLVMCGP1Desc
  CGP1MCAsmInfo.cpp
  CGP1MCInstPrinter.cpp
  CGP1MCCodeEmitter.cpp
  CGP1AsmBackend.cpp
  CGP1ELFObjectWriter.cpp
  CGP1MCTargetDesc.cpp

  LINK_COMPONENTS
  MC
  Support
  CGP1Info

  ADD_TO_COMPONENT
  CGP1
  )

