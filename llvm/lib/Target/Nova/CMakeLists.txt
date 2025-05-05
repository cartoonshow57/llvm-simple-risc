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

#@s cmake-reg-info end
tablegen(LLVM NovaGenRegisterInfo.inc -gen-register-info)
#- cmake-reg-info
#- nova-cmake

add_llvm_target(NovaCodeGen
  NovaTargetMachine.cpp
#@s cmake-reg-info-cpp end
  NovaRegisterInfo.cpp
#- cmake-reg-info-cpp
#@s cmake-mc-tdesc end
  MCTargetDesc/NovaMCTargetDesc.cpp
#- cmake-mc-tdesc

  LINK_COMPONENTS
  MC
  TargetParser
  Target

  ADD_TO_COMPONENT
  Nova
)
