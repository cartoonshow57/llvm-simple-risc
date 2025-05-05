//@s reginfo-cpp1
#include "NovaRegisterInfo.h"
#include "MCTargetDesc/NovaMCTargetDesc.h"
#include "NovaFrameLowering.h"

#include "llvm/ADT/BitVector.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"
// #include "NovaFrameLowering.h"

using namespace llvm;

#define DEBUG_TYPE "nova-reg-info"

#define GET_REGINFO_TARGET_DESC
#include "NovaGenRegisterInfo.inc"

using namespace llvm;

NovaRegisterInfo::NovaRegisterInfo() : NovaGenRegisterInfo(Nova::RA) {}

const MCPhysReg *
NovaRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  static const MCPhysReg CSRList[] = {
      Nova::SP, // Stack Pointer
      Nova::FP, // Frame Pointer
      Nova::S0, // R0
  };
  return CSRList;
}

BitVector NovaRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  static const MCPhysReg ReservedRegs[] = {
      Nova::ZERO, Nova::K0, Nova::K1, Nova::SP,
      Nova::FP, // reserve this for now,
                // but this can be used as a general purpose register
                // if frame pointer is not used in a function.
      Nova::GP  // Reserve only if small section is used.
  };
  BitVector Reserved(getNumRegs());
  for (MCPhysReg Reg : ReservedRegs) {
    Reserved.set(Reg);
  }
  return Reserved;
}

bool NovaRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                           int SPAdj, unsigned FIOperandNum,
                                           RegScavenger *RS) const {
  return true;
}

Register NovaRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  return Nova::FP;
}

//- reginfo-cpp1
