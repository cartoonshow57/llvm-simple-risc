//@s cgp1-target-machine-1 end
#ifndef LLVM_LIB_TARGET_CGP1_CGP1TARGETMACHINE_H
#define LLVM_LIB_TARGET_CGP1_CGP1TARGETMACHINE_H

#include "CGP1Subtarget.h"
#include "CGP1TargetObjectFile.h"
#include "llvm/CodeGen/CodeGenTargetMachineImpl.h"
#include "llvm/Support/CodeGen.h"
#include "llvm/Target/TargetLoweringObjectFile.h"
#include "llvm/Target/TargetMachine.h"

using namespace llvm;

// Data layout for CGP1 GPU (32-bit big endian)
// E = big endian
// m:m = name mangling
// p:32:32 = default (private) pointers are 32-bit
// p1:32:32 = global memory pointers (address space 1)
// p3:32:32 = shared/local memory pointers (address space 3)
// p4:32:32 = constant memory pointers (address space 4)
// n32 = only 32-bit native integers
// S64 = 64-bit natural stack alignment
static const char *CGP1DataLayoutString =
    "E-m:m-p:32:32-p1:32:32-p3:32:32-p4:32:32-i8:8:32-i16:16:32-i64:64-n32-S64";

static Reloc::Model getEffectiveRelocModel(std::optional<Reloc::Model> RM) {
  if (!RM)
    return Reloc::Static;
  return *RM;
}
//- cgp1-target-machine-1

//@s cgp1-target-machine-2
namespace llvm {

class CGP1TargetMachine final : public CodeGenTargetMachineImpl {
  CGP1Subtarget Subtarget;
  std::unique_ptr<TargetLoweringObjectFile> TLOF;

public:
  CGP1TargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                    StringRef FS, const TargetOptions &Options,
                    std::optional<Reloc::Model> RM,
                    std::optional<CodeModel::Model> CM, CodeGenOptLevel OL,
                    bool JIT)
      : CodeGenTargetMachineImpl(T, CGP1DataLayoutString, TT, CPU, FS, Options,
                                 getEffectiveRelocModel(RM),
                                 getEffectiveCodeModel(CM, CodeModel::Medium),
                                 OL),
        Subtarget(TT, CPU, FS, *this), TLOF(new CGP1TargetObjectFile()) {
    initAsmInfo();
  }

  // ~CGP1TargetMachine() override = default;

  TargetLoweringObjectFile *getObjFileLowering() const override {
    return TLOF.get();
  }

  const CGP1Subtarget *getSubtargetImpl(const Function &F) const override {
    return &Subtarget;
  }

  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;
};
} // namespace llvm

#endif
//- cgp1-target-machine-2