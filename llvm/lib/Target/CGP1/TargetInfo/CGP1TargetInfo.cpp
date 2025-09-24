//===-- CGP1TargetInfo.cpp - CGP1 Target Implementation -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "TargetInfo/CGP1TargetInfo.h"
#include "llvm/TargetParser/Triple.h" // optional but safe
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Compiler.h"

using namespace llvm;

Target &llvm::getTheCGP1Target() {
  static Target TheCGP1Target;
  return TheCGP1Target;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeCGP1TargetInfo() {
  // Use UnknownArch for now to avoid having to modify LLVM's Triple enum.
  // If you want a "real" 'cgp1' triple later, add it to LLVM's Triple
  // definitions.
  RegisterTarget<Triple::UnknownArch> X(
      getTheCGP1Target(),
      "cgp1", // name passed to registry (triple arch string)
      "CGP1", // short description
      "CGP1"  // long description
  );
}
