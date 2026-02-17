//@s cgp1-frame-lowering-1
//===--  ----//
// This file defines the frame lowering for the CGP1 target
//===---------------------------------------------------------

#ifndef LLVM_LIB_TARGET_CGP1_FRAMELOWERING_H
#define LLVM_LIB_TARGET_CGP1_FRAMELOWERING_H

#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"

namespace llvm {
class CGP1FrameLowering : public TargetFrameLowering {
public:
  explicit CGP1FrameLowering(const TargetSubtargetInfo &STI, Align Alignment)
      : TargetFrameLowering(StackGrowsDown, Alignment, 0, Alignment) {}

  void emitPrologue(MachineFunction &MF,
                    MachineBasicBlock &MBB) const override;
  void emitEpilogue(MachineFunction &MF,
                    MachineBasicBlock &MBB) const override;

  bool hasFP(const MachineFunction &MF) const override { return true; }

  // Indicate that we have call frame pseudo instructions
  bool hasReservedCallFrame(const MachineFunction &MF) const override {
    return true;
  }

  MachineBasicBlock::iterator
  eliminateCallFramePseudoInstr(MachineFunction &MF, MachineBasicBlock &MBB,
                                MachineBasicBlock::iterator MI) const override;

  // Determine which callee-saved registers need to be saved
  void determineCalleeSaves(MachineFunction &MF, BitVector &SavedRegs,
                            RegScavenger *RS = nullptr) const override;

  // Save callee-saved registers to the stack
  bool spillCalleeSavedRegisters(MachineBasicBlock &MBB,
                                 MachineBasicBlock::iterator MI,
                                 ArrayRef<CalleeSavedInfo> CSI,
                                 const TargetRegisterInfo *TRI) const override;

  // Restore callee-saved registers from the stack
  bool restoreCalleeSavedRegisters(MachineBasicBlock &MBB,
                                   MachineBasicBlock::iterator MI,
                                   MutableArrayRef<CalleeSavedInfo> CSI,
                                   const TargetRegisterInfo *TRI) const override;
};

} // end namespace llvm

#endif
//- cgp1-frame-lowering-1