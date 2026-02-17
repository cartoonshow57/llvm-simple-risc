//@s cgp1-target-object-file.h
#ifndef LLVM_LIB_TARGET_CGP1_CGP1TARGETOBJECTFILE_H
#define LLVM_LIB_TARGET_CGP1_CGP1TARGETOBJECTFILE_H

#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"

namespace llvm {
class TargetMachine;
class MCContext;
class CGP1TargetObjectFile final : public TargetLoweringObjectFileELF {
public:
  void Initialize(MCContext &Ctx, const TargetMachine &TM) override;
};
} // namespace llvm

#endif
//- cgp1-target-object-file.h
