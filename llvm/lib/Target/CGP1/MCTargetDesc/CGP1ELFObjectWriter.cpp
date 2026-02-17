//===-- CGP1ELFObjectWriter.cpp - CGP1 ELF Writer -------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "CGP1MCTargetDesc.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCValue.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

namespace {

class CGP1ELFObjectWriter : public MCELFObjectTargetWriter {
public:
  CGP1ELFObjectWriter(uint8_t OSABI);

  ~CGP1ELFObjectWriter() override = default;

  unsigned getRelocType(MCContext &Ctx, const MCValue &Target,
                        const MCFixup &Fixup, bool IsPCRel) const override;
};

} // end anonymous namespace

CGP1ELFObjectWriter::CGP1ELFObjectWriter(uint8_t OSABI)
    : MCELFObjectTargetWriter(/*Is64Bit*/ false, OSABI, 
                              0xCE71,  // CGP1-specific machine type (custom)
                              /*HasRelocationAddend*/ true) {}

unsigned CGP1ELFObjectWriter::getRelocType(MCContext &Ctx,
                                           const MCValue &Target,
                                           const MCFixup &Fixup,
                                           bool IsPCRel) const {
  // CGP1-specific relocation types (using generic ELF relocation numbers)
  // These should be documented for the hardware team
  unsigned Kind = Fixup.getKind();
  switch (Kind) {
  default:
    return ELF::R_386_NONE;  // Unknown fixup
  case FK_Data_4:
    return IsPCRel ? ELF::R_386_PC32 : ELF::R_386_32;
  case FK_PCRel_4:
    return ELF::R_386_PC32;
  case FK_Data_2:
    return ELF::R_386_16;
  case FK_Data_1:
    return ELF::R_386_8;
  }
}

MCObjectTargetWriter *llvm::createCGP1ELFObjectWriter(const Triple &TT) {
  uint8_t OSABI = MCELFObjectTargetWriter::getOSABI(TT.getOS());
  return new CGP1ELFObjectWriter(OSABI);
}

