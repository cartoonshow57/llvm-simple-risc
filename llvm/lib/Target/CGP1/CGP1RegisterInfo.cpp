//===-- CGP1RegisterInfo.cpp - CGP1 Register Information ------------------===//
#include "CGP1RegisterInfo.h"
#include "MCTargetDesc/CGP1MCTargetDesc.h"
#include "CGP1FrameLowering.h"
#include "CGP1Subtarget.h"

#include "llvm/ADT/BitVector.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"

using namespace llvm;

#define DEBUG_TYPE "cgp1-reg-info"

#define GET_REGINFO_TARGET_DESC
#include "CGP1GenRegisterInfo.inc"

CGP1RegisterInfo::CGP1RegisterInfo() : CGP1GenRegisterInfo(CGP1::R14) {}  // R14 = LR

const MCPhysReg *
CGP1RegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  // Callee-saved registers for CGP1 GPU
  // R4-R11 are callee-saved (general purpose)
  // R12 = FP, R14 = LR
  static const MCPhysReg CSRList[] = {
      CGP1::R4, CGP1::R5, CGP1::R6, CGP1::R7,
      CGP1::R8, CGP1::R9, CGP1::R10, CGP1::R11,
      CGP1::R12,  // Frame Pointer
      CGP1::R14,  // Link Register
      0           // Null terminator
  };
  return CSRList;
}

const uint32_t *
CGP1RegisterInfo::getCallPreservedMask(const MachineFunction &MF,
                                       CallingConv::ID CC) const {
  // Call-preserved register mask
  static const uint32_t CSRMask[] = {
      0xFFFF0000,
      0
  };
  return CSRMask;
}

BitVector CGP1RegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  // Reserved registers:
  // R0  - Zero/scratch
  // R13 - Stack Pointer
  // R14 - Link Register
  // R15 - SIMT EXEC mask (reserved)
  static const MCPhysReg ReservedRegs[] = {
      CGP1::R0,   // Zero/scratch
      CGP1::R13,  // Stack Pointer
      CGP1::R14,  // Link Register
      CGP1::R15   // EXEC mask
  };
  BitVector Reserved(getNumRegs());
  for (MCPhysReg Reg : ReservedRegs) {
    Reserved.set(Reg);
  }
  return Reserved;
}

bool CGP1RegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                           int SPAdj, unsigned FIOperandNum,
                                           RegScavenger *RS) const {
  MachineInstr &MI = *II;
  MachineFunction &MF = *MI.getParent()->getParent();
  MachineFrameInfo &MFI = MF.getFrameInfo();
  const CGP1FrameLowering *TFI = getFrameLowering(MF);

  int FrameIndex = MI.getOperand(FIOperandNum).getIndex();
  int64_t Offset = MFI.getObjectOffset(FrameIndex) + SPAdj;
  
  // Use FP if available, otherwise use SP
  Register BaseReg;
  if (TFI->hasFP(MF)) {
    // Frame pointer points to the saved FP, so offsets are relative to FP
    BaseReg = CGP1::R12;  // FP
    // Offset is already correct (relative to FP)
  } else {
    // Stack pointer: offset includes stack size
    BaseReg = CGP1::R13;  // SP
    Offset += MFI.getStackSize();
  }

  // Replace frame index with base register (FP or SP)
  MI.getOperand(FIOperandNum).ChangeToRegister(BaseReg, false);
  
  if (FIOperandNum + 1 < MI.getNumOperands() && 
      MI.getOperand(FIOperandNum + 1).isImm()) {
    int64_t ImmOffset = MI.getOperand(FIOperandNum + 1).getImm();
    MI.getOperand(FIOperandNum + 1).setImm(Offset + ImmOffset);
  }

  return false;
}

Register CGP1RegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  return CGP1::R12;  // FP = R12
}
