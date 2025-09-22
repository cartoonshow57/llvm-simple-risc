//=====-- CGP1MCAsmInfo.h - CGP1 asm properties -----------*- C++ -*--====//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the CGP1MCAsmInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_CGP1_MCTARGETDESC_CGP1MCASMINFO_H
#define LLVM_LIB_TARGET_CGP1_MCTARGETDESC_CGP1MCASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {
class Triple;

class CGP1MCAsmInfo : public MCAsmInfoELF {
  void anchor() override;

public:
  explicit CGP1MCAsmInfo(const Triple &TheTriple,
                          const MCTargetOptions &Options);
  void printSpecifierExpr(raw_ostream &OS,
                          const MCSpecifierExpr &Expr) const override;
};

namespace CGP1 {
using Specifier = uint8_t;
enum { S_None, S_ABS_HI, S_ABS_LO };
} // namespace CGP1

} // namespace llvm

#endif // LLVM_LIB_TARGET_CGP1_MCTARGETDESC_CGP1MCASMINFO_H
