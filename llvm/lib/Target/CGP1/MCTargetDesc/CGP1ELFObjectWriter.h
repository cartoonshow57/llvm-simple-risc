//===-- CGP1ELFObjectWriter.h - CGP1 ELF Writer ---------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_CGP1_MCTARGETDESC_CGP1ELFOBJECTWRITER_H
#define LLVM_LIB_TARGET_CGP1_MCTARGETDESC_CGP1ELFOBJECTWRITER_H

#include "llvm/MC/MCObjectWriter.h"

namespace llvm {

class Triple;
class MCObjectTargetWriter;

MCObjectTargetWriter *createCGP1ELFObjectWriter(const Triple &TT);

} // end namespace llvm

#endif

