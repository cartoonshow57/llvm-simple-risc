//@s reg-info1 end
///// NovaRegisterInfo.h
//// This contains the Nova implementation of the TargetRegisterInfo class.

#ifndef LLVM_LIB_TARGET_NOVA_NOVAREGISTERINFO_H
#define LLVM_LIB_TARGET_NOVA_NOVAREGISTERINFO_H

#include "llvm/CodeGen/TargetRegisterInfo.h"

#define GET_REGINFO_HEADER
#include "NovaGenRegisterInfo.inc"

// This includes the following:
// #include "llvm/CodeGen/TargetRegisterInfo.h"
// 
// struct NovaGenRegisterInfo : public TargetRegisterInfo;
// namespace Nova { // Register classes
  // extern const TargetRegisterClass GPR32RegClass;
  // ... and others
// }

//- reg-info1

//@s reg-info2 end
namespace llvm {

class NovaRegisterInfo final : public NovaGenRegisterInfo {
public:
  NovaRegisterInfo();
  const MCPhysReg* getCalleeSavedRegs(const MachineFunction *MF) const override;

  BitVector getReservedRegs(const MachineFunction &MF) const override;

  bool eliminateFrameIndex(MachineBasicBlock::iterator II,
                            int SPAdj, unsigned FIOperandNum,
                            RegScavenger *RS = nullptr) const override;

  Register getFrameRegister(const MachineFunction &MF) const override;
};

} // end namespace llvm

#endif
//- reg-info2
