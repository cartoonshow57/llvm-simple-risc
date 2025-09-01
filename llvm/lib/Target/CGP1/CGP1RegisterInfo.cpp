//===-- CGP1RegisterInfo.cpp - CGP1 Register Information ------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the CGP1 implementation of the TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#include "CGP1RegisterInfo.h"
#include "CGP1AluCode.h"
#include "CGP1CondCode.h"
#include "CGP1FrameLowering.h"
#include "CGP1InstrInfo.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/Support/ErrorHandling.h"

#define GET_REGINFO_TARGET_DESC
#include "CGP1GenRegisterInfo.inc"

using namespace llvm;

CGP1RegisterInfo::CGP1RegisterInfo() : CGP1GenRegisterInfo(CGP1::RCA) {}

const uint16_t *
CGP1RegisterInfo::getCalleeSavedRegs(const MachineFunction * /*MF*/) const {
  return CSR_SaveList;
}

BitVector CGP1RegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());

  Reserved.set(CGP1::R0);
  Reserved.set(CGP1::R1);
  Reserved.set(CGP1::PC);
  Reserved.set(CGP1::R2);
  Reserved.set(CGP1::SP);
  Reserved.set(CGP1::R4);
  Reserved.set(CGP1::FP);
  Reserved.set(CGP1::R5);
  Reserved.set(CGP1::RR1);
  Reserved.set(CGP1::R10);
  Reserved.set(CGP1::RR2);
  Reserved.set(CGP1::R11);
  Reserved.set(CGP1::RCA);
  Reserved.set(CGP1::R15);
  if (hasBasePointer(MF))
    Reserved.set(getBaseRegister());
  return Reserved;
}

bool CGP1RegisterInfo::requiresRegisterScavenging(
    const MachineFunction & /*MF*/) const {
  return true;
}

static bool isALUArithLoOpcode(unsigned Opcode) {
  switch (Opcode) {
  case CGP1::ADD_I_LO:
  case CGP1::SUB_I_LO:
  case CGP1::ADD_F_I_LO:
  case CGP1::SUB_F_I_LO:
  case CGP1::ADDC_I_LO:
  case CGP1::SUBB_I_LO:
  case CGP1::ADDC_F_I_LO:
  case CGP1::SUBB_F_I_LO:
    return true;
  default:
    return false;
  }
}

static unsigned getOppositeALULoOpcode(unsigned Opcode) {
  switch (Opcode) {
  case CGP1::ADD_I_LO:
    return CGP1::SUB_I_LO;
  case CGP1::SUB_I_LO:
    return CGP1::ADD_I_LO;
  case CGP1::ADD_F_I_LO:
    return CGP1::SUB_F_I_LO;
  case CGP1::SUB_F_I_LO:
    return CGP1::ADD_F_I_LO;
  case CGP1::ADDC_I_LO:
    return CGP1::SUBB_I_LO;
  case CGP1::SUBB_I_LO:
    return CGP1::ADDC_I_LO;
  case CGP1::ADDC_F_I_LO:
    return CGP1::SUBB_F_I_LO;
  case CGP1::SUBB_F_I_LO:
    return CGP1::ADDC_F_I_LO;
  default:
    llvm_unreachable("Invalid ALU lo opcode");
  }
}

static unsigned getRRMOpcodeVariant(unsigned Opcode) {
  switch (Opcode) {
  case CGP1::LDBs_RI:
    return CGP1::LDBs_RR;
  case CGP1::LDBz_RI:
    return CGP1::LDBz_RR;
  case CGP1::LDHs_RI:
    return CGP1::LDHs_RR;
  case CGP1::LDHz_RI:
    return CGP1::LDHz_RR;
  case CGP1::LDW_RI:
    return CGP1::LDW_RR;
  case CGP1::STB_RI:
    return CGP1::STB_RR;
  case CGP1::STH_RI:
    return CGP1::STH_RR;
  case CGP1::SW_RI:
    return CGP1::SW_RR;
  default:
    llvm_unreachable("Opcode has no RRM variant");
  }
}

bool CGP1RegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                            int SPAdj, unsigned FIOperandNum,
                                            RegScavenger *RS) const {
  assert(SPAdj == 0 && "Unexpected");

  MachineInstr &MI = *II;
  MachineFunction &MF = *MI.getParent()->getParent();
  const TargetInstrInfo *TII = MF.getSubtarget().getInstrInfo();
  const TargetFrameLowering *TFI = MF.getSubtarget().getFrameLowering();
  bool HasFP = TFI->hasFP(MF);
  DebugLoc DL = MI.getDebugLoc();

  int FrameIndex = MI.getOperand(FIOperandNum).getIndex();

  int Offset = MF.getFrameInfo().getObjectOffset(FrameIndex) +
               MI.getOperand(FIOperandNum + 1).getImm();

  // Addressable stack objects are addressed using neg. offsets from fp
  // or pos. offsets from sp/basepointer
  if (!HasFP || (hasStackRealignment(MF) && FrameIndex >= 0))
    Offset += MF.getFrameInfo().getStackSize();

  Register FrameReg = getFrameRegister(MF);
  if (FrameIndex >= 0) {
    if (hasBasePointer(MF))
      FrameReg = getBaseRegister();
    else if (hasStackRealignment(MF))
      FrameReg = CGP1::SP;
  }

  // Replace frame index with a frame pointer reference.
  // If the offset is small enough to fit in the immediate field, directly
  // encode it.
  // Otherwise scavenge a register and encode it into a MOVHI, OR_I_LO sequence.
  if ((isSPLSOpcode(MI.getOpcode()) && !isInt<10>(Offset)) ||
      !isInt<16>(Offset)) {
    assert(RS && "Register scavenging must be on");
    Register Reg = RS->FindUnusedReg(&CGP1::GPRRegClass);
    if (!Reg)
      Reg = RS->scavengeRegisterBackwards(CGP1::GPRRegClass, II, false, SPAdj);
    assert(Reg && "Register scavenger failed");

    bool HasNegOffset = false;
    // ALU ops have unsigned immediate values. If the Offset is negative, we
    // negate it here and reverse the opcode later.
    if (Offset < 0) {
      HasNegOffset = true;
      Offset = -Offset;
    }

    if (!isInt<16>(Offset)) {
      // Reg = hi(offset) | lo(offset)
      BuildMI(*MI.getParent(), II, DL, TII->get(CGP1::MOVHI), Reg)
          .addImm(static_cast<uint32_t>(Offset) >> 16);
      BuildMI(*MI.getParent(), II, DL, TII->get(CGP1::OR_I_LO), Reg)
          .addReg(Reg)
          .addImm(Offset & 0xffffU);
    } else {
      // Reg = mov(offset)
      BuildMI(*MI.getParent(), II, DL, TII->get(CGP1::ADD_I_LO), Reg)
          .addImm(0)
          .addImm(Offset);
    }
    // Reg = FrameReg OP Reg
    if (MI.getOpcode() == CGP1::ADD_I_LO) {
      BuildMI(*MI.getParent(), II, DL,
              HasNegOffset ? TII->get(CGP1::SUB_R) : TII->get(CGP1::ADD_R),
              MI.getOperand(0).getReg())
          .addReg(FrameReg)
          .addReg(Reg)
          .addImm(LPCC::ICC_T);
      MI.eraseFromParent();
      return true;
    }
    if (isSPLSOpcode(MI.getOpcode()) || isRMOpcode(MI.getOpcode())) {
      MI.setDesc(TII->get(getRRMOpcodeVariant(MI.getOpcode())));
      if (HasNegOffset) {
        // Change the ALU op (operand 3) from LPAC::ADD (the default) to
        // LPAC::SUB with the already negated offset.
        assert((MI.getOperand(3).getImm() == LPAC::ADD) &&
               "Unexpected ALU op in RRM instruction");
        MI.getOperand(3).setImm(LPAC::SUB);
      }
    } else
      llvm_unreachable("Unexpected opcode in frame index operation");

    MI.getOperand(FIOperandNum).ChangeToRegister(FrameReg, /*isDef=*/false);
    MI.getOperand(FIOperandNum + 1)
        .ChangeToRegister(Reg, /*isDef=*/false, /*isImp=*/false,
                          /*isKill=*/true);
    return false;
  }

  // ALU arithmetic ops take unsigned immediates. If the offset is negative,
  // we replace the instruction with one that inverts the opcode and negates
  // the immediate.
  if ((Offset < 0) && isALUArithLoOpcode(MI.getOpcode())) {
    unsigned NewOpcode = getOppositeALULoOpcode(MI.getOpcode());
    // We know this is an ALU op, so we know the operands are as follows:
    // 0: destination register
    // 1: source register (frame register)
    // 2: immediate
    BuildMI(*MI.getParent(), II, DL, TII->get(NewOpcode),
            MI.getOperand(0).getReg())
        .addReg(FrameReg)
        .addImm(-Offset);
    MI.eraseFromParent();
    return true;
  }

  MI.getOperand(FIOperandNum).ChangeToRegister(FrameReg, /*isDef=*/false);
  MI.getOperand(FIOperandNum + 1).ChangeToImmediate(Offset);
  return false;
}

bool CGP1RegisterInfo::hasBasePointer(const MachineFunction &MF) const {
  const MachineFrameInfo &MFI = MF.getFrameInfo();
  // When we need stack realignment and there are dynamic allocas, we can't
  // reference off of the stack pointer, so we reserve a base pointer.
  if (hasStackRealignment(MF) && MFI.hasVarSizedObjects())
    return true;

  return false;
}

unsigned CGP1RegisterInfo::getRARegister() const { return CGP1::RCA; }

Register
CGP1RegisterInfo::getFrameRegister(const MachineFunction & /*MF*/) const {
  return CGP1::FP;
}

Register CGP1RegisterInfo::getBaseRegister() const { return CGP1::R14; }

const uint32_t *
CGP1RegisterInfo::getCallPreservedMask(const MachineFunction & /*MF*/,
                                        CallingConv::ID /*CC*/) const {
  return CSR_RegMask;
}
