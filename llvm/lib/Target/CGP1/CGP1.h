//===-- CGP1.h - Top-level interface for CGP1 representation --*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in the LLVM
// CGP1 back-end.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_CGP1_CGP1_H
#define LLVM_LIB_TARGET_CGP1_CGP1_H

#include "llvm/Pass.h"

namespace llvm {
class FunctionPass;
class CGP1TargetMachine;
class PassRegistry;

// createCGP1ISelDag - This pass converts a legalized DAG into a
// CGP1-specific DAG, ready for instruction scheduling.
FunctionPass *createCGP1ISelDag(CGP1TargetMachine &TM);

// createCGP1DelaySlotFillerPass - This pass fills delay slots
// with useful instructions or nop's
FunctionPass *createCGP1DelaySlotFillerPass(const CGP1TargetMachine &TM);

// createCGP1MemAluCombinerPass - This pass combines loads/stores and
// arithmetic operations.
FunctionPass *createCGP1MemAluCombinerPass();

// createCGP1SetflagAluCombinerPass - This pass combines SET_FLAG and ALU
// operations.
FunctionPass *createCGP1SetflagAluCombinerPass();

void initializeCGP1AsmPrinterPass(PassRegistry &);
void initializeCGP1DAGToDAGISelLegacyPass(PassRegistry &);
void initializeCGP1MemAluCombinerPass(PassRegistry &);

} // namespace llvm

#endif // LLVM_LIB_TARGET_CGP1_CGP1_H
