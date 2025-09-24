//===-- CGP1MCExpr.h - CGP1 specific MC expression kinds -------*- C++ -*-===//
//
// Minimal specifier enum for CGP1 used by MCAsmInfo printing.
// This does NOT implement a full MCExpr subclass — it only provides
// the specifier enum values used by printSpecifierExpr.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_CGP1_MCTARGETDESC_CGP1MCEXPR_H
#define LLVM_LIB_TARGET_CGP1_MCTARGETDESC_CGP1MCEXPR_H

namespace CGP1 {
/// Relocation specifier enum used by printSpecifierExpr.
/// Values are arbitrary but must match whatever you emit in your
/// MC/Assembler/Reloc handling (if you later implement full MCExprs).
enum SpecifierKind {
  S_NONE = 0,
  S_ABS_HI = 1,
  S_ABS_LO = 2,
};
} // namespace CGP1

#endif // LLVM_LIB_TARGET_CGP1_MCTARGETDESC_CGP1MCEXPR_H
