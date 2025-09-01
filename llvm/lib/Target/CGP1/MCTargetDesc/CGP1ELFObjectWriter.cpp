//===-- CGP1ELFObjectWriter.cpp - CGP1 ELF Writer -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/CGP1BaseInfo.h"
#include "MCTargetDesc/CGP1FixupKinds.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

namespace {

class CGP1ELFObjectWriter : public MCELFObjectTargetWriter {
public:
  explicit CGP1ELFObjectWriter(uint8_t OSABI);

  ~CGP1ELFObjectWriter() override = default;

protected:
  unsigned getRelocType(const MCFixup &, const MCValue &,
                        bool IsPCRel) const override;
  bool needsRelocateWithSymbol(const MCValue &, unsigned Type) const override;
};

} // end anonymous namespace

CGP1ELFObjectWriter::CGP1ELFObjectWriter(uint8_t OSABI)
    : MCELFObjectTargetWriter(/*Is64Bit_=*/false, OSABI, ELF::EM_CGP1,
                              /*HasRelocationAddend_=*/true) {}

unsigned CGP1ELFObjectWriter::getRelocType(const MCFixup &Fixup,
                                            const MCValue &, bool) const {
  unsigned Type;
  unsigned Kind = static_cast<unsigned>(Fixup.getKind());
  switch (Kind) {
  case CGP1::FIXUP_CGP1_21:
    Type = ELF::R_CGP1_21;
    break;
  case CGP1::FIXUP_CGP1_21_F:
    Type = ELF::R_CGP1_21_F;
    break;
  case CGP1::FIXUP_CGP1_25:
    Type = ELF::R_CGP1_25;
    break;
  case CGP1::FIXUP_CGP1_32:
  case FK_Data_4:
    Type = ELF::R_CGP1_32;
    break;
  case CGP1::FIXUP_CGP1_HI16:
    Type = ELF::R_CGP1_HI16;
    break;
  case CGP1::FIXUP_CGP1_LO16:
    Type = ELF::R_CGP1_LO16;
    break;
  case CGP1::FIXUP_CGP1_NONE:
    Type = ELF::R_CGP1_NONE;
    break;

  default:
    llvm_unreachable("Invalid fixup kind!");
  }
  return Type;
}

bool CGP1ELFObjectWriter::needsRelocateWithSymbol(const MCValue &,
                                                   unsigned Type) const {
  switch (Type) {
  case ELF::R_CGP1_21:
  case ELF::R_CGP1_21_F:
  case ELF::R_CGP1_25:
  case ELF::R_CGP1_32:
  case ELF::R_CGP1_HI16:
    return true;
  default:
    return false;
  }
}

std::unique_ptr<MCObjectTargetWriter>
llvm::createCGP1ELFObjectWriter(uint8_t OSABI) {
  return std::make_unique<CGP1ELFObjectWriter>(OSABI);
}
