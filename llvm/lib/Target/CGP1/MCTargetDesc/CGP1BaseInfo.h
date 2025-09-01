//===-- CGP1BaseInfo.h - Top level definitions for CGP1 MC ----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains small standalone helper functions and enum definitions for
// the CGP1 target useful for the compiler back-end and the MC libraries.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_CGP1_MCTARGETDESC_CGP1BASEINFO_H
#define LLVM_LIB_TARGET_CGP1_MCTARGETDESC_CGP1BASEINFO_H

#include "CGP1MCTargetDesc.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/Support/DataTypes.h"
#include "llvm/Support/ErrorHandling.h"

namespace llvm {

// CGP1II - This namespace holds all of the target specific flags that
// instruction info tracks.
namespace CGP1II {
// Target Operand Flag enum.
enum TOF {
  //===------------------------------------------------------------------===//
  // CGP1 Specific MachineOperand flags.
  MO_NO_FLAG,

  // MO_ABS_HI/LO - Represents the hi or low part of an absolute symbol
  // address.
  MO_ABS_HI,
  MO_ABS_LO,
};
} // namespace CGP1II

static inline unsigned getCGP1RegisterNumbering(MCRegister Reg) {
  switch (Reg.id()) {
  case CGP1::R0:
    return 0;
  case CGP1::R1:
    return 1;
  case CGP1::R2:
  case CGP1::PC:
    return 2;
  case CGP1::R3:
    return 3;
  case CGP1::R4:
  case CGP1::SP:
    return 4;
  case CGP1::R5:
  case CGP1::FP:
    return 5;
  case CGP1::R6:
    return 6;
  case CGP1::R7:
    return 7;
  case CGP1::R8:
  case CGP1::RV:
    return 8;
  case CGP1::R9:
    return 9;
  case CGP1::R10:
  case CGP1::RR1:
    return 10;
  case CGP1::R11:
  case CGP1::RR2:
    return 11;
  case CGP1::R12:
    return 12;
  case CGP1::R13:
    return 13;
  case CGP1::R14:
    return 14;
  case CGP1::R15:
  case CGP1::RCA:
    return 15;
  case CGP1::R16:
    return 16;
  case CGP1::R17:
    return 17;
  case CGP1::R18:
    return 18;
  case CGP1::R19:
    return 19;
  case CGP1::R20:
    return 20;
  case CGP1::R21:
    return 21;
  case CGP1::R22:
    return 22;
  case CGP1::R23:
    return 23;
  case CGP1::R24:
    return 24;
  case CGP1::R25:
    return 25;
  case CGP1::R26:
    return 26;
  case CGP1::R27:
    return 27;
  case CGP1::R28:
    return 28;
  case CGP1::R29:
    return 29;
  case CGP1::R30:
    return 30;
  case CGP1::R31:
    return 31;
  default:
    llvm_unreachable("Unknown register number!");
  }
}
} // namespace llvm
#endif // LLVM_LIB_TARGET_CGP1_MCTARGETDESC_CGP1BASEINFO_H
