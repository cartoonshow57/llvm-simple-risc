//@s cgp1-instr-info.cpp
#include "CGP1InstrInfo.h"
#include "MCTargetDesc/CGP1MCTargetDesc.h"
#include "CGP1TargetMachine.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineFrameInfo.h"

using namespace llvm;

#define DEBUG_TYPE "cgp1-instr-info"

#define GET_INSTRINFO_CTOR_DTOR
#include "CGP1GenInstrInfo.inc"

CGP1InstrInfo::CGP1InstrInfo(const CGP1Subtarget &STI) :
  CGP1GenInstrInfo(CGP1::ADJCALLSTACKDOWN, CGP1::ADJCALLSTACKUP),
  Subtarget(STI) { }

/// copyPhysReg - Copy between physical registers
/// Uses COPY_REG instruction for register copy
void CGP1InstrInfo::copyPhysReg(MachineBasicBlock &MBB,
                                MachineBasicBlock::iterator I,
                                const DebugLoc &DL, Register DestReg,
                                Register SrcReg, bool KillSrc,
                                bool RenamableDest, bool RenamableSrc) const {
  // Check if both registers are GPR (unified register file)
  if (CGP1::GPRRegClass.contains(DestReg, SrcReg)) {
    // Use COPY_REG for register copy
    BuildMI(MBB, I, DL, get(CGP1::COPY_REG), DestReg)
        .addReg(SrcReg, getKillRegState(KillSrc));
    return;
  }

  // Fallback for GPR32 compatibility
  if (CGP1::GPR32RegClass.contains(DestReg) &&
      CGP1::GPR32RegClass.contains(SrcReg)) {
    BuildMI(MBB, I, DL, get(CGP1::COPY_REG), DestReg)
        .addReg(SrcReg, getKillRegState(KillSrc));
    return;
  }

  llvm_unreachable("Cannot copy between these register classes");
}

/// storeRegToStackSlot - Store a register to a stack slot
/// Uses STORE instruction
void CGP1InstrInfo::storeRegToStackSlot(MachineBasicBlock &MBB,
                                        MachineBasicBlock::iterator I,
                                        Register SrcReg, bool isKill,
                                        int FrameIndex,
                                        const TargetRegisterClass *RC,
                                        const TargetRegisterInfo *TRI,
                                        Register VReg,
                                        MachineInstr::MIFlag Flags) const {
  DebugLoc DL;
  if (I != MBB.end())
    DL = I->getDebugLoc();

  if (RC == &CGP1::GPRRegClass || RC == &CGP1::GPR32RegClass) {
    // STORE to stack
    BuildMI(MBB, I, DL, get(CGP1::STORE))
        .addReg(SrcReg, getKillRegState(isKill))
        .addFrameIndex(FrameIndex)
        .addImm(0)
        .setMIFlag(Flags);
  } else {
    llvm_unreachable("Can't store this register to stack slot");
  }
}

/// loadRegFromStackSlot - Load a register from a stack slot
/// Uses LOAD instruction
void CGP1InstrInfo::loadRegFromStackSlot(MachineBasicBlock &MBB,
                                         MachineBasicBlock::iterator I,
                                         Register DestReg, int FrameIndex,
                                         const TargetRegisterClass *RC,
                                         const TargetRegisterInfo *TRI,
                                         Register VReg,
                                         MachineInstr::MIFlag Flags) const {
  DebugLoc DL;
  if (I != MBB.end())
    DL = I->getDebugLoc();

  if (RC == &CGP1::GPRRegClass || RC == &CGP1::GPR32RegClass) {
    // LOAD from stack
    BuildMI(MBB, I, DL, get(CGP1::LOAD), DestReg)
        .addFrameIndex(FrameIndex)
        .addImm(0)
        .setMIFlag(Flags);
  } else {
    llvm_unreachable("Can't load this register from stack slot");
  }
}

//- cgp1-instr-info.cpp
