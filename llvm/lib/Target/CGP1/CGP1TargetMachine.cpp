//===-- CGP1TargetMachine.cpp - Define TargetMachine for CGP1 ---------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Implements the info about CGP1 target spec.
//
//===----------------------------------------------------------------------===//

#include "CGP1TargetMachine.h"

#include "CGP1.h"
#include "CGP1MachineFunctionInfo.h"
#include "CGP1TargetObjectFile.h"
#include "CGP1TargetTransformInfo.h"
#include "TargetInfo/CGP1TargetInfo.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Compiler.h"
#include "llvm/Target/TargetOptions.h"
#include <optional>

using namespace llvm;

extern "C" LLVM_ABI LLVM_EXTERNAL_VISIBILITY void LLVMInitializeCGP1Target() {
  // Register the target.
  RegisterTargetMachine<CGP1TargetMachine> registered_target(
      getTheCGP1Target());
  PassRegistry &PR = *PassRegistry::getPassRegistry();
  initializeCGP1AsmPrinterPass(PR);
  initializeCGP1DAGToDAGISelLegacyPass(PR);
  initializeCGP1MemAluCombinerPass(PR);
}

static std::string computeDataLayout() {
  // Data layout (keep in sync with clang/lib/Basic/Targets.cpp)
  return "E"        // Big endian
         "-m:e"     // ELF name manging
         "-p:32:32" // 32-bit pointers, 32 bit aligned
         "-i64:64"  // 64 bit integers, 64 bit aligned
         "-a:0:32"  // 32 bit alignment of objects of aggregate type
         "-n32"     // 32 bit native integer width
         "-S64";    // 64 bit natural stack alignment
}

static Reloc::Model getEffectiveRelocModel(std::optional<Reloc::Model> RM) {
  return RM.value_or(Reloc::PIC_);
}

CGP1TargetMachine::CGP1TargetMachine(
    const Target &T, const Triple &TT, StringRef Cpu, StringRef FeatureString,
    const TargetOptions &Options, std::optional<Reloc::Model> RM,
    std::optional<CodeModel::Model> CodeModel, CodeGenOptLevel OptLevel,
    bool JIT)
    : CodeGenTargetMachineImpl(
          T, computeDataLayout(), TT, Cpu, FeatureString, Options,
          getEffectiveRelocModel(RM),
          getEffectiveCodeModel(CodeModel, CodeModel::Medium), OptLevel),
      Subtarget(TT, Cpu, FeatureString, *this, Options, getCodeModel(),
                OptLevel),
      TLOF(new CGP1TargetObjectFile()) {
  initAsmInfo();
}

TargetTransformInfo
CGP1TargetMachine::getTargetTransformInfo(const Function &F) const {
  return TargetTransformInfo(std::make_unique<CGP1TTIImpl>(this, F));
}

MachineFunctionInfo *CGP1TargetMachine::createMachineFunctionInfo(
    BumpPtrAllocator &Allocator, const Function &F,
    const TargetSubtargetInfo *STI) const {
  return CGP1MachineFunctionInfo::create<CGP1MachineFunctionInfo>(Allocator,
                                                                    F, STI);
}

namespace {
// CGP1 Code Generator Pass Configuration Options.
class CGP1PassConfig : public TargetPassConfig {
public:
  CGP1PassConfig(CGP1TargetMachine &TM, PassManagerBase *PassManager)
      : TargetPassConfig(TM, *PassManager) {}

  CGP1TargetMachine &getCGP1TargetMachine() const {
    return getTM<CGP1TargetMachine>();
  }

  void addIRPasses() override;
  bool addInstSelector() override;
  void addPreSched2() override;
  void addPreEmitPass() override;
};
} // namespace

TargetPassConfig *
CGP1TargetMachine::createPassConfig(PassManagerBase &PassManager) {
  return new CGP1PassConfig(*this, &PassManager);
}

void CGP1PassConfig::addIRPasses() {
  addPass(createAtomicExpandLegacyPass());

  TargetPassConfig::addIRPasses();
}

// Install an instruction selector pass.
bool CGP1PassConfig::addInstSelector() {
  addPass(createCGP1ISelDag(getCGP1TargetMachine()));
  return false;
}

// Implemented by targets that want to run passes immediately before
// machine code is emitted.
void CGP1PassConfig::addPreEmitPass() {
  addPass(createCGP1DelaySlotFillerPass(getCGP1TargetMachine()));
}

// Run passes after prolog-epilog insertion and before the second instruction
// scheduling pass.
void CGP1PassConfig::addPreSched2() {
  addPass(createCGP1MemAluCombinerPass());
}
