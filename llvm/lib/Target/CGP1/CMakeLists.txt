add_llvm_component_group(CGP1)

set(LLVM_TARGET_DEFINITIONS CGP1.td)

tablegen(LLVM CGP1GenAsmMatcher.inc -gen-asm-matcher)
tablegen(LLVM CGP1GenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM CGP1GenCallingConv.inc -gen-callingconv)
tablegen(LLVM CGP1GenDAGISel.inc -gen-dag-isel)
tablegen(LLVM CGP1GenDisassemblerTables.inc -gen-disassembler)
tablegen(LLVM CGP1GenInstrInfo.inc -gen-instr-info)
tablegen(LLVM CGP1GenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM CGP1GenRegisterInfo.inc -gen-register-info)
tablegen(LLVM CGP1GenSDNodeInfo.inc -gen-sd-node-info)
tablegen(LLVM CGP1GenSubtargetInfo.inc -gen-subtarget)

add_public_tablegen_target(CGP1CommonTableGen)

add_llvm_target(CGP1CodeGen
  CGP1AsmPrinter.cpp
  CGP1DelaySlotFiller.cpp
  CGP1FrameLowering.cpp
  CGP1InstrInfo.cpp
  CGP1ISelDAGToDAG.cpp
  CGP1ISelLowering.cpp
  CGP1MachineFunctionInfo.cpp
  CGP1MCInstLower.cpp
  CGP1MemAluCombiner.cpp
  CGP1RegisterInfo.cpp
  CGP1SelectionDAGInfo.cpp
  CGP1Subtarget.cpp
  CGP1TargetMachine.cpp
  CGP1TargetObjectFile.cpp

  LINK_COMPONENTS
  Analysis
  AsmPrinter
  CodeGen
  CodeGenTypes
  Core
  CGP1AsmParser
  CGP1Desc
  CGP1Info
  MC
  SelectionDAG
  Support
  Target
  TargetParser
  TransformUtils

  ADD_TO_COMPONENT
  CGP1
  )

add_subdirectory(AsmParser)
add_subdirectory(Disassembler)
add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
