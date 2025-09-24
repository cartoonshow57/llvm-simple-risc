//===-- CGP1MCAsmInfo.h - CGP1 asm properties -----------------*- C++ -*-===//
// Minimal MCAsmInfo for CGP1
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_CGP1_MCTARGETDESC_CGP1MCASMINFO_H
#define LLVM_LIB_TARGET_CGP1_MCTARGETDESC_CGP1MCASMINFO_H

#include "llvm/MC/MCAsmInfo.h"        // base class
#include "llvm/MC/MCExpr.h"           // MCExpr (complete type)
#include "llvm/Support/raw_ostream.h" // raw_ostream (complete type)

namespace llvm {
class Triple;
class MCTargetOptions;

/// CGP1-specific MCAsmInfo. Minimal and stable across LLVM versions.
class CGP1MCAsmInfo : public MCAsmInfo {
public:
  // Anchor to force vtable emission in this TU.
  void anchor();

  // Constructor used by target registration.
  CGP1MCAsmInfo(const Triple &TheTriple, const MCTargetOptions &Options);

  // Print target-specific relocation specifiers like %hi/%lo etc.
  // Use MCExpr (LLVM-provided) so the type is always available.
  void printSpecifierExpr(raw_ostream &OS, const MCExpr &Expr) const;
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_CGP1_MCTARGETDESC_CGP1MCASMINFO_H
