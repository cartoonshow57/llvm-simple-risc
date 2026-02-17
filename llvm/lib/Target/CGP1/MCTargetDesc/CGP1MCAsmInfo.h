//@s cgp1-mc-asm.h
#ifndef LLVM_LIB_TARGET_CGP1_MCTARGETDESC_CGP1MCASMINFO_H
#define LLVM_LIB_TARGET_CGP1_MCTARGETDESC_CGP1MCASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {
  class Triple;
class CGP1MCAsmInfo final : public MCAsmInfoELF {
  void anchor() override;
public:
  explicit CGP1MCAsmInfo(const Triple &TheTriple);
};
} // end namespace llvm

#endif
//- cgp1-mc-asm.h
