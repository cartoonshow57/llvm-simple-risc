//@s cgp1-instr-info.h
#ifndef LLVM_LIB_TARGET_CGP1_CGP1INSTRINFO_H
#define LLVM_LIB_TARGET_CGP1_CGP1INSTRINFO_H

#include "CGP1.h"
#include "CGP1RegisterInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "CGP1GenInstrInfo.inc"

namespace llvm {
class CGP1Subtarget;

class CGP1InstrInfo : public CGP1GenInstrInfo {
public:
  explicit CGP1InstrInfo(const CGP1Subtarget &STI);

  /// copyPhysReg - Copy between physical registers
  void copyPhysReg(MachineBasicBlock &MBB, MachineBasicBlock::iterator MI,
                   const DebugLoc &DL, Register DestReg, Register SrcReg,
                   bool KillSrc, bool RenamableDest = false,
                   bool RenamableSrc = false) const override;

  /// storeRegToStackSlot - Store a register to the stack
  void storeRegToStackSlot(MachineBasicBlock &MBB,
                           MachineBasicBlock::iterator MI,
                           Register SrcReg, bool isKill, int FrameIndex,
                           const TargetRegisterClass *RC,
                           const TargetRegisterInfo *TRI,
                           Register VReg,
                           MachineInstr::MIFlag Flags = MachineInstr::NoFlags) const override;

  /// loadRegFromStackSlot - Load a register from the stack
  void loadRegFromStackSlot(MachineBasicBlock &MBB,
                            MachineBasicBlock::iterator MI,
                            Register DestReg, int FrameIndex,
                            const TargetRegisterClass *RC,
                            const TargetRegisterInfo *TRI,
                            Register VReg,
                            MachineInstr::MIFlag Flags = MachineInstr::NoFlags) const override;

protected:
  const CGP1Subtarget &Subtarget;
};
} // end namespace llvm

#endif
//- cgp1-instr-info.h