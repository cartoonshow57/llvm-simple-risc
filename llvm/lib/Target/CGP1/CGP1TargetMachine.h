//===-- CGP1TargetMachine.h - Define TargetMachine for CGP1 --- C++ ---===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares the CGP1 specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_CGP1_CGP1TARGETMACHINE_H
#define LLVM_LIB_TARGET_CGP1_CGP1TARGETMACHINE_H

#include "CGP1ISelLowering.h"
#include "CGP1InstrInfo.h"
#include "CGP1SelectionDAGInfo.h"
#include "CGP1Subtarget.h"
#include "llvm/CodeGen/CodeGenTargetMachineImpl.h"
#include <optional>

namespace llvm {

class CGP1TargetMachine : public CodeGenTargetMachineImpl {
  CGP1Subtarget Subtarget;
  std::unique_ptr<TargetLoweringObjectFile> TLOF;

public:
  CGP1TargetMachine(const Target &TheTarget, const Triple &TargetTriple,
                     StringRef Cpu, StringRef FeatureString,
                     const TargetOptions &Options,
                     std::optional<Reloc::Model> RM,
                     std::optional<CodeModel::Model> CodeModel,
                     CodeGenOptLevel OptLevel, bool JIT);

  const CGP1Subtarget *
  getSubtargetImpl(const llvm::Function & /*Fn*/) const override {
    return &Subtarget;
  }

  TargetTransformInfo getTargetTransformInfo(const Function &F) const override;

  // Pass Pipeline Configuration
  TargetPassConfig *createPassConfig(PassManagerBase &pass_manager) override;

  TargetLoweringObjectFile *getObjFileLowering() const override {
    return TLOF.get();
  }

  MachineFunctionInfo *
  createMachineFunctionInfo(BumpPtrAllocator &Allocator, const Function &F,
                            const TargetSubtargetInfo *STI) const override;

  bool isMachineVerifierClean() const override {
    return false;
  }
};
} // namespace llvm

#endif // LLVM_LIB_TARGET_CGP1_CGP1TARGETMACHINE_H
