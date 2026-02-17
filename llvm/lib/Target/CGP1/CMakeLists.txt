add_llvm_component_group(CGP1)

set(LLVM_TARGET_DEFINITIONS CGP1.td)

tablegen(LLVM CGP1GenRegisterInfo.inc -gen-register-info)
tablegen(LLVM CGP1GenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM CGP1GenInstrInfo.inc -gen-instr-info)
tablegen(LLVM CGP1GenDAGISel.inc -gen-dag-isel)
tablegen(LLVM CGP1GenAsmWriter.inc -gen-asm-writer)

add_public_tablegen_target(CGP1CommonTableGen)

add_llvm_target(CGP1CodeGen
  CGP1AsmPrinter.cpp
  CGP1FrameLowering.cpp
  CGP1InstrInfo.cpp
  CGP1ISelDAGToDAG.cpp
  CGP1ISelLowering.cpp
  CGP1RegisterInfo.cpp
  CGP1SIMTReconvergence.cpp
  CGP1Subtarget.cpp
  CGP1TargetMachine.cpp
  CGP1TargetObjectFile.cpp

  LINK_COMPONENTS
  Analysis
  AsmPrinter
  CodeGen
  CodeGenTypes
  Core
  MC
  CGP1Desc
  CGP1Info
  SelectionDAG
  Support
  Target
  TargetParser

  ADD_TO_COMPONENT
  CGP1
  )

add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
