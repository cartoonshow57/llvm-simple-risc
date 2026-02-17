//===-- CGP1AsmBackend.cpp - CGP1 Asm Backend  ----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the CGP1AsmBackend class.
//
//===----------------------------------------------------------------------===//

#include "CGP1AsmBackend.h"
#include "CGP1MCTargetDesc.h"
#include "CGP1ELFObjectWriter.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCAssembler.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCFixupKindInfo.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCValue.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/MathExtras.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

// Prepare value for the target space for it
static unsigned adjustFixupValue(const MCFixup &Fixup, uint64_t Value,
                                 MCContext &Ctx) {
  unsigned Kind = Fixup.getKind();

  // Handle basic fixup kinds
  switch (Kind) {
  default:
    return 0;
  case FK_Data_1:
    return Value & 0xff;
  case FK_Data_2:
    return Value & 0xffff;
  case FK_Data_4:
    return Value & 0xffffffff;
  case FK_Data_8:
    return Value;
  case FK_PCRel_4:
    // PC-relative 32-bit: value is already adjusted, just mask it
    if (!isInt<32>(Value)) {
      Ctx.reportError(Fixup.getLoc(), "out of range PC-relative fixup");
      return 0;
    }
    return Value & 0xffffffff;
  }
}

void CGP1AsmBackend::applyFixup(const MCAssembler &Asm, const MCFixup &Fixup,
                                const MCValue &Target,
                                MutableArrayRef<char> Data, uint64_t Value,
                                bool IsResolved,
                                const MCSubtargetInfo *STI) const {
  unsigned Kind = Fixup.getKind();
  unsigned NumBytes = 4; // CGP1 instructions are 32-bit

  // Adjust the value according to the fixup kind
  Value = adjustFixupValue(Fixup, Value, const_cast<MCContext &>(Asm.getContext()));

  // Get the fixup offset
  unsigned Offset = Fixup.getOffset();
  assert(Offset + NumBytes <= Data.size() && "Invalid fixup offset!");

  // Write the fixup value in big-endian format
  for (unsigned I = 0; I != NumBytes; ++I) {
    Data[Offset + I] = uint8_t(Value >> (8 * (NumBytes - 1 - I)));
  }
}

std::unique_ptr<MCObjectTargetWriter>
CGP1AsmBackend::createObjectTargetWriter() const {
  return std::unique_ptr<MCObjectTargetWriter>(createCGP1ELFObjectWriter(TheTriple));
}

std::optional<MCFixupKind> CGP1AsmBackend::getFixupKind(StringRef Name) const {
  return std::nullopt;
}

const MCFixupKindInfo &CGP1AsmBackend::getFixupKindInfo(MCFixupKind Kind) const {
  const static MCFixupKindInfo Infos[] = {
      // This table *must* be in the order that the fixup_* kinds are defined in
      // CGP1FixupKinds.h.
      //
      // name                      offset bits  flags
      {"fixup_CGP1_invalid", 0, 0, 0},
  };

  if (Kind < FirstTargetFixupKind)
    return MCAsmBackend::getFixupKindInfo(Kind);

  assert(unsigned(Kind - FirstTargetFixupKind) < getNumFixupKinds() &&
         "Invalid kind!");
  return Infos[Kind - FirstTargetFixupKind];
}

bool CGP1AsmBackend::writeNopData(raw_ostream &OS, uint64_t Count,
                                  const MCSubtargetInfo *STI) const {
  // Write zeros for alignment
  OS.write_zeros(Count);
  return true;
}


