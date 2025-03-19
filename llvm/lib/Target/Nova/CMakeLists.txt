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


add_llvm_component_group(Nova)

#@s nova-cmake end
set(LLVM_TARGET_DEFINITIONS Nova.td)

tablegen(LLVM NovaGenRegisterInfo.inc -gen-register-info)
#- nova-cmake

add_llvm_target(NovaCodeGen
  MipsNovaTargetMachine.cpp

  LINK_COMPONENTS
  MC
  TargetParser
  Target

  ADD_TO_COMPONENT
  Nova
)
