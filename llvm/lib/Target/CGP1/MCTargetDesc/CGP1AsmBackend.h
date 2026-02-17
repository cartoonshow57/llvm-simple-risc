//===-- CGP1AsmBackend.h - CGP1 Asm Backend  ------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the CGP1AsmBackend class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_CGP1_MCTARGETDESC_CGP1ASMBACKEND_H
#define LLVM_LIB_TARGET_CGP1_MCTARGETDESC_CGP1ASMBACKEND_H

#include "llvm/MC/MCAsmBackend.h"
#include "llvm/TargetParser/Triple.h"

namespace llvm {

class MCAssembler;
struct MCFixupKindInfo;
class MCRegisterInfo;
class Target;

class CGP1AsmBackend : public MCAsmBackend {
  Triple TheTriple;

public:
  CGP1AsmBackend(const Target &T, const MCRegisterInfo &MRI, const Triple &TT)
      : MCAsmBackend(llvm::endianness::big), // CGP1 is big endian
        TheTriple(TT) {}

  std::unique_ptr<MCObjectTargetWriter>
  createObjectTargetWriter() const override;

  void applyFixup(const MCAssembler &Asm, const MCFixup &Fixup,
                  const MCValue &Target, MutableArrayRef<char> Data,
                  uint64_t Value, bool IsResolved,
                  const MCSubtargetInfo *STI) const override;

  std::optional<MCFixupKind> getFixupKind(StringRef Name) const override;
  const MCFixupKindInfo &getFixupKindInfo(MCFixupKind Kind) const override;

  unsigned getNumFixupKinds() const override {
    return 1; // Basic fixup kinds
  }

  bool writeNopData(raw_ostream &OS, uint64_t Count,
                    const MCSubtargetInfo *STI) const override;
};

} // namespace llvm

#endif

