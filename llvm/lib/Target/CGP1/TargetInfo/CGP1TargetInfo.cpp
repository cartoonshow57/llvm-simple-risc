//===-- CGP1TargetInfo.cpp - CGP1 Target Implementation -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "TargetInfo/CGP1TargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Compiler.h"

using namespace llvm;

Target &llvm::getTheCGP1Target() {
  static Target TheCGP1Target;
  return TheCGP1Target;
}

extern "C" LLVM_ABI LLVM_EXTERNAL_VISIBILITY void
LLVMInitializeCGP1TargetInfo() {
  RegisterTarget<Triple::cgp1> X(getTheCGP1Target(), "cgp1", "CGP1",
                                  "CGP1");
}
