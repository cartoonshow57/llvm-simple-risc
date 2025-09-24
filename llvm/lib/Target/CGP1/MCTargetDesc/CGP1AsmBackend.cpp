//===-- CGP1AsmBackend.cpp - CGP1 Assembler Backend ---------------------===//
//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "CGP1FixupKinds.h"
#include "MCTargetDesc/CGP1MCTargetDesc.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCAsmLayout.h"
#include "llvm/MC/MCAssembler.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCFixupKindInfo.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCRelaxableFragment.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

// Prepare value for the target space (tiny shim).
static unsigned adjustFixupValue(unsigned Kind, uint64_t Value) {
  switch (Kind) {
  case FK_Data_1:
  case FK_Data_2:
  case FK_Data_4:
  case FK_Data_8:
    return (unsigned)Value;
  case CGP1::FIXUP_CGP1_21:
  case CGP1::FIXUP_CGP1_21_F:
  case CGP1::FIXUP_CGP1_25:
  case CGP1::FIXUP_CGP1_32:
  case CGP1::FIXUP_CGP1_HI16:
  case CGP1::FIXUP_CGP1_LO16:
    return (unsigned)Value;
  default:
    llvm_unreachable("Unknown fixup kind!");
  }
}

namespace {

class CGP1AsmBackend : public MCAsmBackend {
  Triple::OSType OSType;

public:
  CGP1AsmBackend(const Target &T, Triple::OSType OST)
      : MCAsmBackend(llvm::endianness::big), OSType(OST) {}

  // Required overrides:
  unsigned getNumFixupKinds() const override {
    return CGP1::NumTargetFixupKinds;
  }

  // Note: signature must match current LLVM: takes MCAssembler & and
  // MCSubtargetInfo*
  void applyFixup(const MCAssembler &Asm, const MCFixup &Fixup,
                  const MCValue &Target, MutableArrayRef<char> Data,
                  uint64_t Value, bool IsResolved,
                  const MCSubtargetInfo *STI) const override;

  std::unique_ptr<MCObjectTargetWriter>
  createObjectTargetWriter() const override;

  // return by reference (must be const&)
  const MCFixupKindInfo &getFixupKindInfo(MCFixupKind Kind) const override;

  // Basic conservative implementation — no relaxation needed by default.
  bool fixupNeedsRelaxation(const MCFixup & /*Fixup*/, uint64_t /*Value*/,
                            const MCRelaxableFragment * /*DF*/,
                            const MCAsmLayout & /*Layout*/) const override {
    return false;
  }

  bool writeNopData(raw_ostream &OS, uint64_t Count,
                    const MCSubtargetInfo *STI) const override;
};

} // end anonymous namespace

// Minimal NOP writer: require 4-byte aligned NOPs for this ISA (example).
bool CGP1AsmBackend::writeNopData(raw_ostream &OS, uint64_t Count,
                                  const MCSubtargetInfo * /*STI*/) const {
  if ((Count % 4) != 0)
    return false;

  for (uint64_t i = 0; i < Count; i += 4)
    OS.write("\x15\0\0\0", 4); // example 4-byte NOP pattern

  return true;
}

// applyFixup must match MCAsmBackend API. This implementation follows your
// previous logic but accepts the MCAssembler & and MCSubtargetInfo *.
void CGP1AsmBackend::applyFixup(const MCAssembler & /*Asm*/,
                                const MCFixup &Fixup, const MCValue &Target,
                                MutableArrayRef<char> Data, uint64_t Value,
                                bool IsResolved,
                                const MCSubtargetInfo * /*STI*/) const {
  MCFixupKind Kind = Fixup.getKind();
  Value = adjustFixupValue(static_cast<unsigned>(Kind), Value);

  if (!Value)
    return; // This value doesn't change the encoding

  unsigned Offset = Fixup.getOffset();
  unsigned NumBytes = (getFixupKindInfo(Kind).TargetSize + 7) / 8;
  unsigned FullSize = 4;

  uint64_t CurVal = 0;
  for (unsigned i = 0; i != NumBytes; ++i) {
    unsigned Idx = (FullSize - 1 - i);
    CurVal |= static_cast<uint64_t>(static_cast<uint8_t>(Data[Offset + Idx]))
              << (i * 8);
  }

  uint64_t Mask =
      (static_cast<uint64_t>(-1) >> (64 - getFixupKindInfo(Kind).TargetSize));
  CurVal |= Value & Mask;

  for (unsigned i = 0; i != NumBytes; ++i) {
    unsigned Idx = (FullSize - 1 - i);
    Data[Offset + Idx] = static_cast<uint8_t>((CurVal >> (i * 8)) & 0xff);
  }
}

std::unique_ptr<MCObjectTargetWriter>
CGP1AsmBackend::createObjectTargetWriter() const {
  return createCGP1ELFObjectWriter(MCELFObjectTargetWriter::getOSABI(OSType));
}

const MCFixupKindInfo &
CGP1AsmBackend::getFixupKindInfo(MCFixupKind Kind) const {
  static const MCFixupKindInfo Infos[CGP1::NumTargetFixupKinds] = {
      // Keep the same order as CGP1FixupKinds.h
      {"FIXUP_CGP1_NONE", 0, 32, 0},
      {"FIXUP_CGP1_21", 16, 16 /*21*/, 0},
      {"FIXUP_CGP1_21_F", 16, 16 /*21*/, 0},
      {"FIXUP_CGP1_25", 7, 25, 0},
      {"FIXUP_CGP1_32", 0, 32, 0},
      {"FIXUP_CGP1_HI16", 16, 16, 0},
      {"FIXUP_CGP1_LO16", 16, 16, 0}};

  if (Kind < FirstTargetFixupKind)
    return MCAsmBackend::getFixupKindInfo(Kind);

  assert(unsigned(Kind - FirstTargetFixupKind) < CGP1::NumTargetFixupKinds &&
         "Invalid kind!");
  return Infos[Kind - FirstTargetFixupKind];
}

// Factory function called by target registration
MCAsmBackend *llvm::createCGP1AsmBackend(const Target &T,
                                         const MCSubtargetInfo &STI,
                                         const MCRegisterInfo & /*MRI*/,
                                         const MCTargetOptions & /*Options*/) {
  const Triple &TT = STI.getTargetTriple();
  if (!TT.isOSBinFormatELF())
    llvm_unreachable("OS not supported");

  return new CGP1AsmBackend(T, TT.getOS());
}
