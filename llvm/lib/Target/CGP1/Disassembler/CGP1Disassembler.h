//===- CGP1Disassembler.cpp - Disassembler for CGP1 -----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file is part of the CGP1 Disassembler.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_CGP1_DISASSEMBLER_CGP1DISASSEMBLER_H
#define LLVM_LIB_TARGET_CGP1_DISASSEMBLER_CGP1DISASSEMBLER_H

#include "llvm/MC/MCDisassembler/MCDisassembler.h"

namespace llvm {

class CGP1Disassembler : public MCDisassembler {
public:
  CGP1Disassembler(const MCSubtargetInfo &STI, MCContext &Ctx);

  ~CGP1Disassembler() override = default;

  // getInstruction - See MCDisassembler.
  MCDisassembler::DecodeStatus
  getInstruction(MCInst &Instr, uint64_t &Size, ArrayRef<uint8_t> Bytes,
                 uint64_t Address, raw_ostream &CStream) const override;
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_CGP1_DISASSEMBLER_CGP1DISASSEMBLER_H
