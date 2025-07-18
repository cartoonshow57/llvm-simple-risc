#ifndef LLVM_LIB_TARGET_SIMPLERISC_MCTARGETDESC_SIMPLERISCMCASMINFO_H
#define LLVM_LIB_TARGET_SIMPLERISC_MCTARGETDESC_SIMPLERISCMCASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {

class Triple;

class SimpleRISCMCAsmInfo : public MCAsmInfoELF {
  void anchor() override;

public:
  explicit SimpleRISCMCAsmInfo(const Triple &TheTriple);
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_SIMPLERISC_MCTARGETDESC_SIMPLERISCMCASMINFO_H
