//===-- CGP1MCCodeEmitter.cpp - Convert CGP1 Code to Machine Code ---------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the CGP1MCCodeEmitter class for the CGP1 GPU ISA.
//
// Instruction Format (32-bit):
// | opcode (5) | cond (3) | k (4) | sel (4) | ra (4) | rb (4) | rc (4) | unused (4) |
// |  31-27     | 26-24    | 23-20 | 19-16   | 15-12  | 11-8   | 7-4    | 3-0        |
//
//===----------------------------------------------------------------------===//

#include "CGP1MCCodeEmitter.h"
#include "CGP1MCTargetDesc.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrDesc.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/EndianStream.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <ctime>

using namespace llvm;

#define DEBUG_TYPE "cgp1-mccodeemitter"

#define GET_INSTRMAP_INFO
#include "CGP1GenInstrInfo.inc"
#undef GET_INSTRMAP_INFO

namespace llvm {

MCCodeEmitter *createCGP1MCCodeEmitter(const MCInstrInfo &MCII,
                                       MCContext &Ctx) {
  return new CGP1MCCodeEmitter(MCII, Ctx);
}

} // end namespace llvm

/// encodeInstruction - Emit the instruction.
void CGP1MCCodeEmitter::encodeInstruction(const MCInst &MI,
                                          SmallVectorImpl<char> &CB,
                                          SmallVectorImpl<MCFixup> &Fixups,
                                          const MCSubtargetInfo &STI) const {
  uint32_t Binary = getBinaryCodeForInstr(MI, Fixups, STI);

  const MCInstrDesc &Desc = MCII.get(MI.getOpcode());
  unsigned Size = Desc.getSize();
  if (!Size)
    Size = 4;  // All CGP1 instructions are 32-bit

  // CGP1 is big endian
  support::endian::write<uint32_t>(CB, Binary, llvm::endianness::big);
}

/// getBranchTargetOpValue - Return binary encoding of branch target.
unsigned CGP1MCCodeEmitter::
getBranchTargetOpValue(const MCInst &MI, unsigned OpNo,
                       SmallVectorImpl<MCFixup> &Fixups,
                       const MCSubtargetInfo &STI) const {
  const MCOperand &MO = MI.getOperand(OpNo);
  if (MO.isImm()) return MO.getImm() >> 2;

  assert(MO.isExpr() && "getBranchTargetOpValue expects expression or immediate");
  Fixups.push_back(MCFixup::create(0, MO.getExpr(), MCFixupKind(FK_PCRel_4)));
  return 0;
}

/// getJumpTargetOpValue - Return binary encoding of jump target.
unsigned CGP1MCCodeEmitter::
getJumpTargetOpValue(const MCInst &MI, unsigned OpNo,
                     SmallVectorImpl<MCFixup> &Fixups,
                     const MCSubtargetInfo &STI) const {
  const MCOperand &MO = MI.getOperand(OpNo);
  if (MO.isImm()) return MO.getImm() >> 2;

  assert(MO.isExpr() && "getJumpTargetOpValue expects expression or immediate");
  Fixups.push_back(MCFixup::create(0, MO.getExpr(), MCFixupKind(FK_Data_4)));
  return 0;
}

/// getMachineOpValue - Return binary encoding of operand.
unsigned CGP1MCCodeEmitter::
getMachineOpValue(const MCInst &MI, const MCOperand &MO,
                  SmallVectorImpl<MCFixup> &Fixups,
                  const MCSubtargetInfo &STI) const {
  if (MO.isReg()) {
    return Ctx.getRegisterInfo()->getEncodingValue(MO.getReg());
  } else if (MO.isImm()) {
    return static_cast<unsigned>(MO.getImm());
  }
  assert(MO.isExpr() && "Expected register, immediate, or expression");
  return 0;
}

/// getMemEncoding - Return binary encoding of memory operand.
unsigned CGP1MCCodeEmitter::
getMemEncoding(const MCInst &MI, unsigned OpNo,
              SmallVectorImpl<MCFixup> &Fixups,
              const MCSubtargetInfo &STI) const {
  assert(MI.getOperand(OpNo).isReg());
  unsigned RegBits = getMachineOpValue(MI, MI.getOperand(OpNo), Fixups, STI) << 16;
  unsigned OffBits = getMachineOpValue(MI, MI.getOperand(OpNo+1), Fixups, STI);
  return (OffBits & 0xFFFF) | RegBits;
}

//===----------------------------------------------------------------------===//
// CGP1 GPU ISA Binary Encoding
//
// Format: | opcode (5) | cond (3) | k (4) | sel (4) | ra (4) | rb (4) | rc (4) | unused (4) |
//         |  31-27     | 26-24    | 23-20 | 19-16   | 15-12  | 11-8   | 7-4    | 3-0        |
//===----------------------------------------------------------------------===//

/// Helper to build instruction encoding
static uint32_t encodeInstr(uint32_t opcode, uint32_t cond, uint32_t k, 
                            uint32_t sel, uint32_t ra, uint32_t rb, uint32_t rc) {
  return ((opcode & 0x1F) << 27) |
         ((cond & 0x7) << 24) |
         ((k & 0xF) << 20) |
         ((sel & 0xF) << 16) |
         ((ra & 0xF) << 12) |
         ((rb & 0xF) << 8) |
         ((rc & 0xF) << 4);
}

uint64_t CGP1MCCodeEmitter::getBinaryCodeForInstr(const MCInst &MI,
                                                   SmallVectorImpl<MCFixup> &Fixups,
                                                   const MCSubtargetInfo &STI) const {
  unsigned Opcode = MI.getOpcode();
  uint32_t Binary = 0;
  
  // #region agent log
  {
    FILE* logfile = fopen("/home/car/llvm-dev/llvm-cgp1/llvm-project/.cursor/debug.log", "a");
    if (logfile) {
      fprintf(logfile, "{\"id\":\"log_%lu_%u\",\"timestamp\":%lu,\"location\":\"CGP1MCCodeEmitter.cpp:146\",\"message\":\"Encoding instruction\",\"data\":{\"opcode\":%u,\"numOperands\":%u},\"sessionId\":\"debug-session\",\"runId\":\"run1\",\"hypothesisId\":\"A\"}\n", 
              (unsigned long)time(nullptr), Opcode, (unsigned long)time(nullptr) * 1000, Opcode, (unsigned)MI.getNumOperands());
      fclose(logfile);
    }
  }
  // #endregion
  
  // Helper to get register encoding (4-bit)
  auto getReg = [&](unsigned OpIdx) -> unsigned {
    if (OpIdx >= MI.getNumOperands()) return 0;
    const MCOperand &MO = MI.getOperand(OpIdx);
    if (MO.isReg()) {
      return Ctx.getRegisterInfo()->getEncodingValue(MO.getReg()) & 0xF;
    }
    return 0;
  };
  
  // Helper to get immediate value
  auto getImm = [&](unsigned OpIdx) -> unsigned {
    if (OpIdx >= MI.getNumOperands()) return 0;
    const MCOperand &MO = MI.getOperand(OpIdx);
    if (MO.isImm()) {
      return static_cast<unsigned>(MO.getImm());
    }
    return 0;
  };

  // Condition code (default: always = 0)
  const uint32_t COND_AL = 0;
  
  switch (Opcode) {
  //===----------------------------------------------------------------------===//
  // NOP (opcode 00000)
  //===----------------------------------------------------------------------===//
  case CGP1::NOP:
    Binary = encodeInstr(0b00000, COND_AL, 0, 0, 0, 0, 0);
    break;

  //===----------------------------------------------------------------------===//
  // Load Immediate Instructions (opcode 00001)
  //===----------------------------------------------------------------------===//
  case CGP1::PLOADF_L:
  case CGP1::PLOADU_L:
  case CGP1::PLOADADDR_L:
    Binary = encodeInstr(0b00001, COND_AL, 0, 0, 0, 0, getReg(0));
    break;
  case CGP1::LOADF_H:
  case CGP1::LOADU_H:
  case CGP1::LOADADDR_H:
    Binary = encodeInstr(0b00001, COND_AL, 0, 0, getReg(1), 0, getReg(0));
    break;

  //===----------------------------------------------------------------------===//
  // Data Movement (opcode 00010)
  //===----------------------------------------------------------------------===//
  case CGP1::COPY_REG:
    // copy rc, ra: opcode=0b00010, sel=0
    Binary = encodeInstr(0b00010, COND_AL, 0, 0b0000, getReg(1), 0, getReg(0));
    break;
  case CGP1::FABS:
    // fabs rc, ra: opcode=0b00010, sel=1
    Binary = encodeInstr(0b00010, COND_AL, 0, 0b0001, getReg(1), 0, getReg(0));
    break;
  case CGP1::FNEG:
    // fneg rc, ra: opcode=0b00010, sel=2
    Binary = encodeInstr(0b00010, COND_AL, 0, 0b0010, getReg(1), 0, getReg(0));
    break;
  case CGP1::FNABS:
    // fnabs rc, ra: opcode=0b00010, sel=3
    Binary = encodeInstr(0b00010, COND_AL, 0, 0b0011, getReg(1), 0, getReg(0));
    break;

  //===----------------------------------------------------------------------===//
  // FP Arithmetic (opcodes 00100-00110)
  //===----------------------------------------------------------------------===//
  case CGP1::FADD:
    // fadd rc, ra, rb: opcode=0b00100, sel=0
    Binary = encodeInstr(0b00100, COND_AL, 0, 0b0000, getReg(1), getReg(2), getReg(0));
    break;
  case CGP1::FSUB:
    // fsub rc, ra, rb: opcode=0b00100, sel=1
    Binary = encodeInstr(0b00100, COND_AL, 0, 0b0001, getReg(1), getReg(2), getReg(0));
    break;
  case CGP1::FMUL:
    // fmul rc, ra, rb: opcode=0b00101
    Binary = encodeInstr(0b00101, COND_AL, 0, 0b0000, getReg(1), getReg(2), getReg(0));
    break;
  case CGP1::FDIV:
    // fdiv rc, ra, rb: opcode=0b00110
    Binary = encodeInstr(0b00110, COND_AL, 0, 0b0000, getReg(1), getReg(2), getReg(0));
    break;
  case CGP1::FRCP:
    // Fast reciprocal: opcode=0b00110, sel=0b0001
    Binary = encodeInstr(0b00110, COND_AL, 0, 0b0001, getReg(1), 0, getReg(0));
    break;
  case CGP1::FRSQRT:
    // Fast reciprocal square root: opcode=0b00110, sel=0b0010
    Binary = encodeInstr(0b00110, COND_AL, 0, 0b0010, getReg(1), 0, getReg(0));
    break;

  //===----------------------------------------------------------------------===//
  // FP Multiply Power of 2 (opcode 00011)
  //===----------------------------------------------------------------------===//
  case CGP1::FMULP2:
    Binary = encodeInstr(0b00011, COND_AL, 0, 0b0000, getReg(1), getReg(2), getReg(0));
    break;

  //===----------------------------------------------------------------------===//
  // FP Utility (opcode 00111)
  //===----------------------------------------------------------------------===//
  case CGP1::FMIN:
    Binary = encodeInstr(0b00111, COND_AL, 0, 0b0000, getReg(1), getReg(2), getReg(0));
    break;
  case CGP1::FMAX:
    Binary = encodeInstr(0b00111, COND_AL, 0, 0b0001, getReg(1), getReg(2), getReg(0));
    break;
  case CGP1::FTRUNC:
    Binary = encodeInstr(0b01000, COND_AL, 0, 0b0000, getReg(1), 0, getReg(0));
    break;
  case CGP1::FROUND:
    Binary = encodeInstr(0b01000, COND_AL, 0, 0b0001, getReg(1), 0, getReg(0));
    break;
  case CGP1::FCEIL:
    Binary = encodeInstr(0b01000, COND_AL, 0, 0b0010, getReg(1), 0, getReg(0));
    break;
  case CGP1::FFLOOR:
    Binary = encodeInstr(0b01000, COND_AL, 0, 0b0011, getReg(1), 0, getReg(0));
    break;
  case CGP1::FSIN:
    // Fast sine: opcode=0b01000, sel=0b0100
    Binary = encodeInstr(0b01000, COND_AL, 0, 0b0100, getReg(1), 0, getReg(0));
    break;
  case CGP1::FCOS:
    // Fast cosine: opcode=0b01000, sel=0b0101
    Binary = encodeInstr(0b01000, COND_AL, 0, 0b0101, getReg(1), 0, getReg(0));
    break;
  case CGP1::FEXP:
    // Fast exponential: opcode=0b01000, sel=0b0110
    Binary = encodeInstr(0b01000, COND_AL, 0, 0b0110, getReg(1), 0, getReg(0));
    break;
  case CGP1::FLOG:
    // Fast logarithm: opcode=0b01000, sel=0b0111
    Binary = encodeInstr(0b01000, COND_AL, 0, 0b0111, getReg(1), 0, getReg(0));
    break;
  case CGP1::FTOU32:
    Binary = encodeInstr(0b01011, COND_AL, 0, 0b0000, getReg(1), 0, getReg(0));
    break;

  //===----------------------------------------------------------------------===//
  // Float to 8-bit Conversion (opcode 01001)
  //===----------------------------------------------------------------------===//
  case CGP1::FTOU8_LL:
    Binary = encodeInstr(0b01001, COND_AL, 0, 0b0000, getReg(1), 0, getReg(0));
    break;
  case CGP1::FTOU8_LH:
    Binary = encodeInstr(0b01001, COND_AL, 0, 0b0001, getReg(1), 0, getReg(0));
    break;
  case CGP1::FTOU8_HL:
    Binary = encodeInstr(0b01001, COND_AL, 0, 0b0010, getReg(1), 0, getReg(0));
    break;
  case CGP1::FTOU8_HH:
    Binary = encodeInstr(0b01001, COND_AL, 0, 0b0011, getReg(1), 0, getReg(0));
    break;

  //===----------------------------------------------------------------------===//
  // Float to 16-bit Conversion (opcode 01010)
  //===----------------------------------------------------------------------===//
  case CGP1::FTOU16_L:
    Binary = encodeInstr(0b01010, COND_AL, 0, 0b0000, getReg(1), 0, getReg(0));
    break;
  case CGP1::FTOU16_H:
    Binary = encodeInstr(0b01010, COND_AL, 0, 0b0001, getReg(1), 0, getReg(0));
    break;

  //===----------------------------------------------------------------------===//
  // 8-bit to Float Conversion (opcode 01100)
  //===----------------------------------------------------------------------===//
  case CGP1::U8TOF_LL:
    Binary = encodeInstr(0b01100, COND_AL, 0, 0b0000, getReg(1), 0, getReg(0));
    break;
  case CGP1::U8TOF_LH:
    Binary = encodeInstr(0b01100, COND_AL, 0, 0b0001, getReg(1), 0, getReg(0));
    break;
  case CGP1::U8TOF_HL:
    Binary = encodeInstr(0b01100, COND_AL, 0, 0b0010, getReg(1), 0, getReg(0));
    break;
  case CGP1::U8TOF_HH:
    Binary = encodeInstr(0b01100, COND_AL, 0, 0b0011, getReg(1), 0, getReg(0));
    break;

  //===----------------------------------------------------------------------===//
  // 16-bit to Float Conversion (opcode 01101)
  //===----------------------------------------------------------------------===//
  case CGP1::U16TOF_L:
    Binary = encodeInstr(0b01101, COND_AL, 0, 0b0000, getReg(1), 0, getReg(0));
    break;
  case CGP1::U16TOF_H:
    Binary = encodeInstr(0b01101, COND_AL, 0, 0b0001, getReg(1), 0, getReg(0));
    break;

  //===----------------------------------------------------------------------===//
  // Integer/FP Conversion (opcode 01110 for u32tof)
  //===----------------------------------------------------------------------===//
  case CGP1::U32TOF:
    Binary = encodeInstr(0b01110, COND_AL, 0, 0b0000, getReg(1), 0, getReg(0));
    break;

  //===----------------------------------------------------------------------===//
  // FP Comparison (opcode 01111)
  //===----------------------------------------------------------------------===//
  case CGP1::FCOMP_L:
    Binary = encodeInstr(0b01111, COND_AL, 0, 0b0000, getReg(1), getReg(2), getReg(0));
    break;
  case CGP1::FCOMP_LE:
    Binary = encodeInstr(0b01111, COND_AL, 0, 0b0001, getReg(1), getReg(2), getReg(0));
    break;
  case CGP1::FCOMP_E:
    Binary = encodeInstr(0b01111, COND_AL, 0, 0b0010, getReg(1), getReg(2), getReg(0));
    break;
  case CGP1::FCOMP_GE:
    Binary = encodeInstr(0b01111, COND_AL, 0, 0b0011, getReg(1), getReg(2), getReg(0));
    break;
  case CGP1::FCOMP_G:
    Binary = encodeInstr(0b01111, COND_AL, 0, 0b0100, getReg(1), getReg(2), getReg(0));
    break;
  case CGP1::FCOMP_NE:
    Binary = encodeInstr(0b01111, COND_AL, 0, 0b0101, getReg(1), getReg(2), getReg(0));
    break;
  case CGP1::FCOMP_U:
    Binary = encodeInstr(0b01111, COND_AL, 0, 0b0110, getReg(1), getReg(2), getReg(0));
    break;

  //===----------------------------------------------------------------------===//
  // Integer Arithmetic (opcode 10000)
  //===----------------------------------------------------------------------===//
  case CGP1::ADD:
    // add rc, ra, rb: opcode=0b10000
    Binary = encodeInstr(0b10000, COND_AL, 0, 0b0000, getReg(1), getReg(2), getReg(0));
    break;
  case CGP1::SUB:
    // sub rc, ra, rb: opcode=0b10000, sel=0b0001
    Binary = encodeInstr(0b10000, COND_AL, 0, 0b0001, getReg(1), getReg(2), getReg(0));
    break;
  case CGP1::MUL:
    // mul rc, ra, rb: opcode=0b10000, sel=0b0010
    Binary = encodeInstr(0b10000, COND_AL, 0, 0b0010, getReg(1), getReg(2), getReg(0));
    break;
  case CGP1::DIV:
    // div rc, ra, rb: opcode=0b10000, sel=0b0011
    Binary = encodeInstr(0b10000, COND_AL, 0, 0b0011, getReg(1), getReg(2), getReg(0));
    break;
  case CGP1::MOD:
    // mod rc, ra, rb: opcode=0b10000, sel=0b0100
    Binary = encodeInstr(0b10000, COND_AL, 0, 0b0100, getReg(1), getReg(2), getReg(0));
    break;
  case CGP1::AND:
    // and rc, ra, rb: opcode=0b10000, sel=0b0101
    Binary = encodeInstr(0b10000, COND_AL, 0, 0b0101, getReg(1), getReg(2), getReg(0));
    break;
  case CGP1::OR:
    // or rc, ra, rb: opcode=0b10000, sel=0b0110
    Binary = encodeInstr(0b10000, COND_AL, 0, 0b0110, getReg(1), getReg(2), getReg(0));
    break;
  case CGP1::XOR:
    // xor rc, ra, rb: opcode=0b10000, sel=0b0111
    Binary = encodeInstr(0b10000, COND_AL, 0, 0b0111, getReg(1), getReg(2), getReg(0));
    break;
  case CGP1::SHL:
    // shl rc, ra, rb: opcode=0b10000, sel=0b1000
    Binary = encodeInstr(0b10000, COND_AL, 0, 0b1000, getReg(1), getReg(2), getReg(0));
    break;
  case CGP1::SHR:
    // shr rc, ra, rb: opcode=0b10000, sel=0b1001
    Binary = encodeInstr(0b10000, COND_AL, 0, 0b1001, getReg(1), getReg(2), getReg(0));
    break;
  case CGP1::SHRA:
    // shra rc, ra, rb: opcode=0b10000, sel=0b1010
    Binary = encodeInstr(0b10000, COND_AL, 0, 0b1010, getReg(1), getReg(2), getReg(0));
    break;

  //===----------------------------------------------------------------------===//
  // Memory Instructions (opcodes 10001-10100)
  //===----------------------------------------------------------------------===//
  case CGP1::LOAD_ADDR:
    // load_addr rc, [ra + offset]: opcode=10001
    Binary = encodeInstr(0b10001, COND_AL, 0, 0, getReg(1), 0, getReg(0));
    break;
  case CGP1::STORE_ADDR:
    // store_addr rb, [ra + offset]: opcode=10010
    Binary = encodeInstr(0b10010, COND_AL, 0, 0, getReg(1), getReg(0), 0);
    break;
  case CGP1::LOAD:
    // load rc, [ra]: opcode=10011
    Binary = encodeInstr(0b10011, COND_AL, 0, 0, getReg(1), 0, getReg(0));
    break;
  case CGP1::STORE:
    // store [ra], rb: opcode=10100
    Binary = encodeInstr(0b10100, COND_AL, 0, 0, getReg(1), getReg(0), 0);
    break;
  case CGP1::PUSH:
    // push rb: opcode=10100
    Binary = encodeInstr(0b10100, COND_AL, 0, 0, 0, getReg(0), 0);
    break;
  case CGP1::POP:
    // pop rc: opcode=10011
    Binary = encodeInstr(0b10011, COND_AL, 0, 0b0001, 0, 0, getReg(0));
    break;
  case CGP1::D_LOAD:
  case CGP1::I_LOAD:
  case CGP1::LOAD_D:
  case CGP1::LOAD_I:
    // load variants: opcode=10011
    Binary = encodeInstr(0b10011, COND_AL, 0, 0, getReg(1), 0, getReg(0));
    break;
  case CGP1::D_STORE:
  case CGP1::I_STORE:
  case CGP1::STORE_D:
  case CGP1::STORE_I:
    // store variants: opcode=10100
    Binary = encodeInstr(0b10100, COND_AL, 0, 0, getReg(1), getReg(0), 0);
    break;

  //===----------------------------------------------------------------------===//
  // Control Flow (opcodes 10101-11000)
  //===----------------------------------------------------------------------===//
  case CGP1::JUMP:
    // jump target: opcode=10101
    Binary = encodeInstr(0b10101, COND_AL, 0, 0, 0, 0, 0);
    // TODO: Add target address encoding
    break;
  case CGP1::BRZ:
    // Branch if zero: opcode=0b10101, cond=EQ (001)
    Binary = encodeInstr(0b10101, 0b001, 0, 0, getReg(0), 0, 0);
    break;
  case CGP1::BRNZ:
    // Branch if not zero: opcode=0b10101, cond=NE (010)
    Binary = encodeInstr(0b10101, 0b010, 0, 0, getReg(0), 0, 0);
    break;
  case CGP1::BRLTZ:
    // Branch if less than zero: opcode=0b10101, cond=LT (011)
    Binary = encodeInstr(0b10101, 0b011, 0, 0, getReg(0), 0, 0);
    break;
  case CGP1::BRGEZ:
    // Branch if greater than or equal to zero: opcode=0b10101, cond=GE (110)
    Binary = encodeInstr(0b10101, 0b110, 0, 0, getReg(0), 0, 0);
    break;
  case CGP1::GOTO:
    // goto target: opcode=10110
    Binary = encodeInstr(0b10110, COND_AL, 0, 0, 0, 0, 0);
    break;
  case CGP1::CALL:
    // call target: opcode=10110
    Binary = encodeInstr(0b10110, COND_AL, 0, 0, 0, 0, 0);
    break;
  case CGP1::RCALL:
    // rcall ra: opcode=10101
    Binary = encodeInstr(0b10101, COND_AL, 0, 0, getReg(0), 0, 0);
    break;
  case CGP1::RET:
  case CGP1::PseudoRet:
    // ret: opcode=10111
    Binary = encodeInstr(0b10111, COND_AL, 0, 0, 0, 0, 0);
    break;
  case CGP1::STOP_CORE:
    // stop_core: opcode=11000
    Binary = encodeInstr(0b11000, COND_AL, 0, 0, 0, 0, 0);
    break;
  case CGP1::IRQ:
    // irq: opcode=11001
    Binary = encodeInstr(0b11001, COND_AL, 0, 0, 0, 0, 0);
    break;

  //===----------------------------------------------------------------------===//
  // GPU Thread/Block Instructions (opcode 11010)
  //===----------------------------------------------------------------------===//
  case CGP1::THREAD_ID_X:
    Binary = encodeInstr(0b11010, COND_AL, 0, 0b0000, 0, 0, getReg(0));
    break;
  case CGP1::THREAD_ID_Y:
    Binary = encodeInstr(0b11010, COND_AL, 0, 0b0001, 0, 0, getReg(0));
    break;
  case CGP1::THREAD_ID_Z:
    Binary = encodeInstr(0b11010, COND_AL, 0, 0b0010, 0, 0, getReg(0));
    break;
  case CGP1::BLOCK_ID_X:
    Binary = encodeInstr(0b11010, COND_AL, 0, 0b0011, 0, 0, getReg(0));
    break;
  case CGP1::BLOCK_ID_Y:
    Binary = encodeInstr(0b11010, COND_AL, 0, 0b0100, 0, 0, getReg(0));
    break;
  case CGP1::BLOCK_ID_Z:
    Binary = encodeInstr(0b11010, COND_AL, 0, 0b0101, 0, 0, getReg(0));
    break;
  case CGP1::BLOCK_DIM_X:
    Binary = encodeInstr(0b11010, COND_AL, 0, 0b0110, 0, 0, getReg(0));
    break;
  case CGP1::BLOCK_DIM_Y:
    Binary = encodeInstr(0b11010, COND_AL, 0, 0b0111, 0, 0, getReg(0));
    break;
  case CGP1::BLOCK_DIM_Z:
    Binary = encodeInstr(0b11010, COND_AL, 0, 0b1000, 0, 0, getReg(0));
    break;
  case CGP1::GRID_DIM_X:
    Binary = encodeInstr(0b11010, COND_AL, 0, 0b1001, 0, 0, getReg(0));
    break;
  case CGP1::GRID_DIM_Y:
    Binary = encodeInstr(0b11010, COND_AL, 0, 0b1010, 0, 0, getReg(0));
    break;
  case CGP1::GRID_DIM_Z:
    Binary = encodeInstr(0b11010, COND_AL, 0, 0b1011, 0, 0, getReg(0));
    break;

  //===----------------------------------------------------------------------===//
  // GPU Synchronization (opcode 11011)
  //===----------------------------------------------------------------------===//
  case CGP1::BARRIER:
    Binary = encodeInstr(0b11011, COND_AL, 0, 0, 0, 0, 0);
    break;

  //===----------------------------------------------------------------------===//
  // GPU Graphics (opcode 11100)
  //===----------------------------------------------------------------------===//
  case CGP1::WRITE_PIXEL:
    // write_pixel x, y, color: sel=0
    Binary = encodeInstr(0b11100, COND_AL, 0, 0b0000, getReg(0), getReg(1), getReg(2));
    break;
  case CGP1::CLEAR_SCREEN:
    // clear_screen color: sel=1
    Binary = encodeInstr(0b11100, COND_AL, 0, 0b0001, getReg(0), 0, 0);
    break;
  case CGP1::DRAW_LINE:
    // draw_line: sel=2 (assumed, check instruction definition)
    Binary = encodeInstr(0b11100, COND_AL, 0, 0b0010, 0, 0, 0);
    break;
  case CGP1::DRAW_TRIANGLE:
    // draw_triangle: sel=3 (assumed, check instruction definition)
    Binary = encodeInstr(0b11100, COND_AL, 0, 0b0011, 0, 0, 0);
    break;

  //===----------------------------------------------------------------------===//
  // Shared Memory Instructions (opcode 11101)
  //===----------------------------------------------------------------------===//
  case CGP1::SHARED_LOAD:
    // shared_load rd, addr: opcode=0b11101, sel=0
    Binary = encodeInstr(0b11101, COND_AL, 0, 0b0000, getReg(1), 0, getReg(0));
    break;
  case CGP1::SHARED_STORE:
    // shared_store addr, val: opcode=0b11101, sel=1
    // Operands: (outs), (ins GPR:$addr, GPR:$val)
    // So: addr=0, val=1
    Binary = encodeInstr(0b11101, COND_AL, 0, 0b0001, getReg(0), getReg(1), 0);
    break;

  //===----------------------------------------------------------------------===//
  // Atomic Instructions (opcode 11110)
  //===----------------------------------------------------------------------===//
  case CGP1::ATOMIC_ADD:
    // atomic_add rd, addr, val: opcode=0b11110, sel=0
    Binary = encodeInstr(0b11110, COND_AL, 0, 0b0000, getReg(1), getReg(2), getReg(0));
    break;
  case CGP1::ATOMIC_CAS:
    // atomic_cas rd, addr, cmp, val: opcode=0b11110, sel=1
    // Operands: (outs GPR:$rd), (ins GPR:$addr, GPR:$cmp, GPR:$val)
    // Format: ra=addr, rb=val, rc=rd (cmp may be encoded in k field or handled differently)
    Binary = encodeInstr(0b11110, COND_AL, getReg(2) & 0xF, 0b0001, getReg(1), getReg(3), getReg(0));
    break;
  case CGP1::ATOMIC_MAX:
    // atomic_max rd, addr, val: opcode=0b11110, sel=2
    Binary = encodeInstr(0b11110, COND_AL, 0, 0b0010, getReg(1), getReg(2), getReg(0));
    break;
  case CGP1::ATOMIC_MIN:
    // atomic_min rd, addr, val: opcode=0b11110, sel=3
    Binary = encodeInstr(0b11110, COND_AL, 0, 0b0011, getReg(1), getReg(2), getReg(0));
    break;
  case CGP1::ATOMIC_EXCH:
    // atomic_exch rd, addr, val: opcode=0b11110, sel=4
    Binary = encodeInstr(0b11110, COND_AL, 0, 0b0100, getReg(1), getReg(2), getReg(0));
    break;

  //===----------------------------------------------------------------------===//
  // Warp-Level Instructions (opcode 11111)
  //===----------------------------------------------------------------------===//
  case CGP1::WARP_SIZE:
    // warp_size rd: opcode=0b11111, sel=0
    Binary = encodeInstr(0b11111, COND_AL, 0, 0b0000, 0, 0, getReg(0));
    break;
  case CGP1::LANE_ID:
    // lane_id rd: opcode=0b11111, sel=1
    Binary = encodeInstr(0b11111, COND_AL, 0, 0b0001, 0, 0, getReg(0));
    break;
  case CGP1::WARP_ALL:
    // warp_all rd, pred: opcode=0b11111, sel=2
    Binary = encodeInstr(0b11111, COND_AL, 0, 0b0010, getReg(1), 0, getReg(0));
    break;
  case CGP1::WARP_ANY:
    // warp_any rd, pred: opcode=0b11111, sel=3
    Binary = encodeInstr(0b11111, COND_AL, 0, 0b0011, getReg(1), 0, getReg(0));
    break;
  case CGP1::WARP_BALLOT:
    // warp_ballot rd, pred: opcode=0b11111, sel=4
    Binary = encodeInstr(0b11111, COND_AL, 0, 0b0100, getReg(1), 0, getReg(0));
    break;
  case CGP1::WARP_SHUFFLE:
    // warp_shuffle rd, val, lane: opcode=0b11111, sel=5
    Binary = encodeInstr(0b11111, COND_AL, 0, 0b0101, getReg(1), getReg(2), getReg(0));
    break;

  //===----------------------------------------------------------------------===//
  // Pseudo Instructions (no binary encoding)
  //===----------------------------------------------------------------------===//
  case CGP1::ADJCALLSTACKDOWN:
  case CGP1::ADJCALLSTACKUP:
  case CGP1::SIMT_BRANCH:
  case CGP1::LOADI:
  case CGP1::LOADFI:
  case CGP1::EXEC_RECONVERGE:
  case CGP1::PseudoRet:
    // These are pseudo instructions, no encoding
    Binary = 0;
    break;

  default:
    // Unknown instruction - return 0
    Binary = 0;
    // #region agent log
    {
      FILE* logfile = fopen("/home/car/llvm-dev/llvm-cgp1/llvm-project/.cursor/debug.log", "a");
      if (logfile) {
        fprintf(logfile, "{\"id\":\"log_%lu_default_%u\",\"timestamp\":%lu,\"location\":\"CGP1MCCodeEmitter.cpp:520\",\"message\":\"Unknown instruction - falling to default\",\"data\":{\"opcode\":%u,\"numOperands\":%u},\"sessionId\":\"debug-session\",\"runId\":\"run1\",\"hypothesisId\":\"A\"}\n",
                (unsigned long)time(nullptr), Opcode, (unsigned long)time(nullptr) * 1000, Opcode, (unsigned)MI.getNumOperands());
        fclose(logfile);
      }
    }
    // #endregion
    break;
  }

  // #region agent log
  {
    FILE* logfile = fopen("/home/car/llvm-dev/llvm-cgp1/llvm-project/.cursor/debug.log", "a");
    if (logfile) {
      fprintf(logfile, "{\"id\":\"log_%lu_result_%u\",\"timestamp\":%lu,\"location\":\"CGP1MCCodeEmitter.cpp:526\",\"message\":\"Encoding result\",\"data\":{\"opcode\":%u,\"binary\":\"0x%08X\"},\"sessionId\":\"debug-session\",\"runId\":\"run1\",\"hypothesisId\":\"A\"}\n",
              (unsigned long)time(nullptr), Opcode, (unsigned long)time(nullptr) * 1000, Opcode, Binary);
      fclose(logfile);
    }
  }
  // #endregion

  return Binary;
}
