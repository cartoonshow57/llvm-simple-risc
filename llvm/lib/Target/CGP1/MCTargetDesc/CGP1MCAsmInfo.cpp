//@s cgp1-mc-asm.cpp
#include "CGP1MCAsmInfo.h"
#include "llvm/TargetParser/Triple.h"

using namespace llvm;

void CGP1MCAsmInfo::anchor() {}

CGP1MCAsmInfo::CGP1MCAsmInfo(const Triple& TT) {
  IsLittleEndian = false;
  AlignmentIsInBytes = true;
}
//- cgp1-mc-asm.cpp
