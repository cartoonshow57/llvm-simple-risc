add_llvm_component_library(LLVMCGP1Desc
  CGP1AsmBackend.cpp
  CGP1ELFObjectWriter.cpp
  CGP1InstPrinter.cpp
  CGP1MCAsmInfo.cpp
  CGP1MCCodeEmitter.cpp
  CGP1MCTargetDesc.cpp

  LINK_COMPONENTS
  CGP1Info
  MC
  MCDisassembler
  Support
  TargetParser

  ADD_TO_COMPONENT
  CGP1
)
