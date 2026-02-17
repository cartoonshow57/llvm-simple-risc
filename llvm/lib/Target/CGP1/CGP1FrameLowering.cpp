#include "CGP1FrameLowering.h"
#include "CGP1InstrInfo.h"
#include "CGP1Subtarget.h"
#include "MCTargetDesc/CGP1MCTargetDesc.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/RegisterScavenging.h"
 #include "llvm/IR/Function.h"

using namespace llvm;

// Stack adjustment is done via ADD instruction with SP
// We materialize the immediate using LOADI (or PLOADU_L/LOADU_H for large values)
// then use ADD to adjust the stack pointer

void CGP1FrameLowering::emitPrologue(MachineFunction &MF,
                                     MachineBasicBlock &MBB) const {
  const Function &F = MF.getFunction();

  // Minimal SIMT model: initialize EXEC mask to all-ones for kernels.
  if (F.getCallingConv() == CallingConv::SPIR_KERNEL ||
      F.hasFnAttribute("cgp1_kernel")) {
    auto It = MBB.begin();
    const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();
    DebugLoc DL = (It != MBB.end()) ? It->getDebugLoc() : DebugLoc();
    BuildMI(MBB, It, DL, TII.get(CGP1::LOADI), CGP1::R15).addImm(-1);
  }

  MachineFrameInfo &MFI = MF.getFrameInfo();
  uint64_t StackSize = MFI.getStackSize();
  
  // Align stack size to 8 bytes
  StackSize = alignTo(StackSize, 8);
  MFI.setStackSize(StackSize);

  if (!StackSize)
    return;

  MachineBasicBlock::iterator MBBI = MBB.begin();
  const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();
  DebugLoc DL = (MBBI != MBB.end()) ? MBBI->getDebugLoc() : DebugLoc();

  // Check if we need to set up frame pointer
  bool NeedsFP = hasFP(MF);
  
  // If using frame pointer, save old SP to FP before adjusting stack
  if (NeedsFP) {
    // FP = SP (save current stack pointer as frame pointer)
    // Use COPY_REG instruction: FP = SP
    BuildMI(MBB, MBBI, DL, TII.get(CGP1::COPY_REG), CGP1::R12)
        .addReg(CGP1::R13);
  }

  // Adjust stack pointer: SP = SP - StackSize
  // Use LOAD_ADDR to adjust SP efficiently if offset fits in 16 bits
  if (isInt<16>(-StackSize)) {
    BuildMI(MBB, MBBI, DL, TII.get(CGP1::LOAD_ADDR), CGP1::SP)
        .addReg(CGP1::SP)
        .addImm(-StackSize);
  } else {
    // For large stack sizes, we need to materialize the value differently
    // Use PLOADU_L and LOADU_H to load 32-bit immediate in two parts
    // Low 16 bits
    BuildMI(MBB, MBBI, DL, TII.get(CGP1::PLOADU_L), CGP1::R1)
        .addImm(-StackSize & 0xFFFF);
    // High 16 bits (sign-extend for negative values)
    int16_t highPart = (-StackSize) >> 16;
    BuildMI(MBB, MBBI, DL, TII.get(CGP1::LOADU_H), CGP1::R1)
        .addReg(CGP1::R1)
        .addImm(highPart);
    // Add SP with scratch register: SP = SP + R1
    BuildMI(MBB, MBBI, DL, TII.get(CGP1::ADD), CGP1::R13)
        .addReg(CGP1::R13)
        .addReg(CGP1::R1, RegState::Kill);
  }
}

void CGP1FrameLowering::emitEpilogue(MachineFunction &MF,
                                     MachineBasicBlock &MBB) const {
  MachineFrameInfo &MFI = MF.getFrameInfo();
  uint64_t StackSize = MFI.getStackSize();
  
  MachineBasicBlock::iterator MBBI = MBB.getLastNonDebugInstr();
  const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();
  DebugLoc DL = (MBBI != MBB.end()) ? MBBI->getDebugLoc() : DebugLoc();

  // If using frame pointer, restore SP from FP: SP = FP
  if (hasFP(MF)) {
    BuildMI(MBB, MBBI, DL, TII.get(CGP1::COPY_REG), CGP1::R13)
        .addReg(CGP1::R12);
  } else {
    // If no FP, restore SP by adding StackSize
    if (!StackSize)
      return;

    // Restore stack pointer: SP = SP + StackSize
    // Use LOAD_ADDR to adjust SP efficiently if offset fits in 16 bits
    if (isInt<16>(StackSize)) {
      BuildMI(MBB, MBBI, DL, TII.get(CGP1::LOAD_ADDR), CGP1::SP)
          .addReg(CGP1::SP)
          .addImm(StackSize);
    } else {
      // For large stack sizes, materialize 32-bit value in two parts
      // Low 16 bits
      BuildMI(MBB, MBBI, DL, TII.get(CGP1::PLOADU_L), CGP1::R1)
          .addImm(StackSize & 0xFFFF);
      // High 16 bits
      int16_t highPart = StackSize >> 16;
      BuildMI(MBB, MBBI, DL, TII.get(CGP1::LOADU_H), CGP1::R1)
          .addReg(CGP1::R1)
          .addImm(highPart);
      // Add SP with scratch register: SP = SP + R1
      BuildMI(MBB, MBBI, DL, TII.get(CGP1::ADD), CGP1::R13)
          .addReg(CGP1::R13)
          .addReg(CGP1::R1, RegState::Kill);
    }
  }
}

void CGP1FrameLowering::determineCalleeSaves(MachineFunction &MF,
                                             BitVector &SavedRegs,
                                             RegScavenger *RS) const {
  TargetFrameLowering::determineCalleeSaves(MF, SavedRegs, RS);

  // If using frame pointer, we need to save FP (R12) as callee-saved
  if (hasFP(MF)) {
    SavedRegs.set(CGP1::R12);  // FP = R12
  }

  // If any function makes a call, we need to save LR (R14)
  MachineFrameInfo &MFI = MF.getFrameInfo();
  if (MFI.hasCalls()) {
    SavedRegs.set(CGP1::R14);  // LR = R14
  }
}

bool CGP1FrameLowering::spillCalleeSavedRegisters(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator MI,
    ArrayRef<CalleeSavedInfo> CSI, const TargetRegisterInfo *TRI) const {
  if (CSI.empty())
    return false;

  MachineFunction *MF = MBB.getParent();
  const TargetInstrInfo &TII = *MF->getSubtarget().getInstrInfo();
  DebugLoc DL = (MI != MBB.end()) ? MI->getDebugLoc() : DebugLoc();

  for (const CalleeSavedInfo &Info : CSI) {
    Register Reg = Info.getReg();
    int FI = Info.getFrameIdx();

    // Use STORE for all registers (unified register file)
    BuildMI(MBB, MI, DL, TII.get(CGP1::STORE))
        .addReg(Reg, RegState::Kill)
        .addFrameIndex(FI)
        .addImm(0);
  }
  return true;
}

bool CGP1FrameLowering::restoreCalleeSavedRegisters(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator MI,
    MutableArrayRef<CalleeSavedInfo> CSI, const TargetRegisterInfo *TRI) const {
  if (CSI.empty())
    return false;

  MachineFunction *MF = MBB.getParent();
  const TargetInstrInfo &TII = *MF->getSubtarget().getInstrInfo();
  DebugLoc DL = (MI != MBB.end()) ? MI->getDebugLoc() : DebugLoc();

  // Restore in reverse order
  for (const CalleeSavedInfo &Info : reverse(CSI)) {
    Register Reg = Info.getReg();
    int FI = Info.getFrameIdx();

    // Use LOAD for all registers (unified register file)
    BuildMI(MBB, MI, DL, TII.get(CGP1::LOAD), Reg)
        .addFrameIndex(FI)
        .addImm(0);
  }
  return true;
}

MachineBasicBlock::iterator CGP1FrameLowering::eliminateCallFramePseudoInstr(
    MachineFunction &MF, MachineBasicBlock &MBB,
    MachineBasicBlock::iterator MI) const {
  // We have a reserved call frame, so we don't need to adjust the stack
  // pointer around calls. Just erase the pseudo instruction.
  return MBB.erase(MI);
}

