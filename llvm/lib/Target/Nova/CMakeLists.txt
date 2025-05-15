#@s nova-cmake1 removed
# add_llvm_component_group(Nova)

# add_llvm_target(NovaCodeGen
#   MipsNovaTargetMachine.cpp

#   LINK_COMPONENTS
#   MC
#   TargetParser
#   Target

#   ADD_TO_COMPONENT
#   Nova
#   )

#- nova-cmake1

#@s cmake-reg-info commented
# tablegen(LLVM NovaGenRegisterInfo.inc -gen-register-info)
# add_public_tablegen_target(NovaCommonTableGen)
#- cmake-reg-info

add_llvm_component_group(Nova)

#@s nova-cmake end
set(LLVM_TARGET_DEFINITIONS Nova.td)

tablegen(LLVM NovaGenRegisterInfo.inc -gen-register-info)

#@s cmake-subtarget-info end
tablegen(LLVM NovaGenSubtargetInfo.inc -gen-subtarget)
#- cmake-subtarget-info
#@s cmake-instr-info end
tablegen(LLVM NovaGenInstrInfo.inc -gen-instr-info)
#- cmake-instr-info
#@s cmake-dag-isel end
tablegen(LLVM NovaGenDAGISel.inc -gen-dag-isel)
#- cmake-dag-isel
#@s cmake-asm-writer end
tablegen(LLVM NovaGenAsmWriter.inc -gen-asm-writer)
#- cmake-asm-writer

add_public_tablegen_target(NovaCommonTableGen)
#- nova-cmake

add_llvm_target(NovaCodeGen
  NovaTargetMachine.cpp
#@s cmake-reg-info-cpp end
  NovaRegisterInfo.cpp
#- cmake-reg-info-cpp
#@s cmake-mc-tdesc end
  MCTargetDesc/NovaMCTargetDesc.cpp
#- cmake-mc-tdesc
#@s cmake-setting-up end
  NovaTargetObjectFile.cpp
  NovaSubtarget.cpp
  MCTargetDesc/NovaMCAsmInfo.cpp
#- cmake-setting-up
#@s cmake-instr-info end
  NovaInstrInfo.cpp
#- cmake-instr-info
#@s cmake-isel-lowering end
  NovaISelLowering.cpp
#- cmake-isel-lowering
#@s cmake-isel-dag
  NovaISelDAGToDAG.cpp
#- cmake-isel-dag
#@s cmake-inst end
  MCTargetDesc/NovaMCInstPrinter.cpp
#- cmake-inst
#@s cmake-asm-printer
  NovaAsmPrinter.cpp
#- cmake-asm-printer

  LINK_COMPONENTS
  MC
  TargetParser
  Target

  ADD_TO_COMPONENT
  Nova
)
