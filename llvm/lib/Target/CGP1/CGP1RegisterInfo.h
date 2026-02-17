//@s reg-info1 end
///// CGP1RegisterInfo.h
//// This contains the CGP1 implementation of the TargetRegisterInfo class.

#ifndef LLVM_LIB_TARGET_CGP1_CGP1REGISTERINFO_H
#define LLVM_LIB_TARGET_CGP1_CGP1REGISTERINFO_H

#include "llvm/CodeGen/TargetRegisterInfo.h"

#define GET_REGINFO_HEADER
#include "CGP1GenRegisterInfo.inc"

// This includes the following:
// #include "llvm/CodeGen/TargetRegisterInfo.h"
// 
// struct CGP1GenRegisterInfo : public TargetRegisterInfo;
// namespace CGP1 { // Register classes
  // extern const TargetRegisterClass GPR32RegClass;
  // ... and others
// }

//- reg-info1

//@s reg-info2 end
namespace llvm {

class CGP1RegisterInfo final : public CGP1GenRegisterInfo {
public:
  CGP1RegisterInfo();
  const MCPhysReg* getCalleeSavedRegs(const MachineFunction *MF) const override;

  const uint32_t *getCallPreservedMask(const MachineFunction &MF,
                                       CallingConv::ID CC) const override;

  BitVector getReservedRegs(const MachineFunction &MF) const override;

  bool eliminateFrameIndex(MachineBasicBlock::iterator II,
                            int SPAdj, unsigned FIOperandNum,
                            RegScavenger *RS = nullptr) const override;

  Register getFrameRegister(const MachineFunction &MF) const override;
};

} // end namespace llvm

#endif
//- reg-info2
