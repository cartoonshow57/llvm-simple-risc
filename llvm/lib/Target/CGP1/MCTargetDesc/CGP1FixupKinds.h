//===-- CGP1FixupKinds.h - CGP1 Specific Fixup Entries --------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_CGP1_MCTARGETDESC_CGP1FIXUPKINDS_H
#define LLVM_LIB_TARGET_CGP1_MCTARGETDESC_CGP1FIXUPKINDS_H

#include "llvm/MC/MCFixup.h"

namespace llvm {
namespace CGP1 {
// Although most of the current fixup types reflect a unique relocation
// one can have multiple fixup types for a given relocation and thus need
// to be uniquely named.
//
// This table *must* be in the save order of
// MCFixupKindInfo Infos[CGP1::NumTargetFixupKinds]
// in CGP1AsmBackend.cpp.
//
enum Fixups {
  // Results in R_CGP1_NONE
  FIXUP_CGP1_NONE = FirstTargetFixupKind,

  FIXUP_CGP1_21,   // 21-bit symbol relocation
  FIXUP_CGP1_21_F, // 21-bit symbol relocation, last two bits masked to 0
  FIXUP_CGP1_25,   // 25-bit branch targets
  FIXUP_CGP1_32,   // general 32-bit relocation
  FIXUP_CGP1_HI16, // upper 16-bits of a symbolic relocation
  FIXUP_CGP1_LO16, // lower 16-bits of a symbolic relocation

  // Marker
  LastTargetFixupKind,
  NumTargetFixupKinds = LastTargetFixupKind - FirstTargetFixupKind
};
} // namespace CGP1
} // namespace llvm

#endif // LLVM_LIB_TARGET_CGP1_MCTARGETDESC_CGP1FIXUPKINDS_H
