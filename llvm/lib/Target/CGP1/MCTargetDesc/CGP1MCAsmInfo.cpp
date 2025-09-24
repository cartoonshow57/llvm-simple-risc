//===-- CGP1MCAsmInfo.cpp - CGP1 asm properties -----------------------===//
// Minimal implementation matching the header.
//
//===----------------------------------------------------------------------===//

#include "CGP1MCAsmInfo.h"
#include "MCTargetDesc/CGP1MCExpr.h" // optional local enum stub (if present)
#include "llvm/MC/MCExpr.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/TargetParser/Triple.h"

using namespace llvm;

void CGP1MCAsmInfo::anchor() {}

CGP1MCAsmInfo::CGP1MCAsmInfo(const Triple & /*TheTriple*/,
                             const MCTargetOptions & /*Options*/) {
  IsLittleEndian = false;
  PrivateGlobalPrefix = ".L";
  WeakRefDirective = "\t.weak\t";
  ExceptionsType = ExceptionHandling::DwarfCFI;
  UsesELFSectionDirectiveForBSS = true;
  CommentString = "!";
  SupportsDebugInformation = true;
  MinInstAlignment = 4;
}

// Minimal, robust printing: use MCExpr::print to print the expression.
// This avoids depending on a non-standard MCSpecifierExpr type.
void CGP1MCAsmInfo::printSpecifierExpr(raw_ostream &OS,
                                       const MCExpr &Expr) const {
  // MCExpr::print has signature like `void print(raw_ostream &, const
  // MCAsmInfo*) const` in many LLVM versions. Try that first; if your LLVM
  // version differs we will adapt.
  Expr.print(OS, this);
}
