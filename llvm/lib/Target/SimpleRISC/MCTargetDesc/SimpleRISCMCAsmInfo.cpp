#include "SimpleRISCMCAsmInfo.h"
#include "llvm/TargetParser/Triple.h" // Corrected include path

using namespace llvm;

void SimpleRISCMCAsmInfo::anchor() {}

SimpleRISCMCAsmInfo::SimpleRISCMCAsmInfo(const Triple &TheTriple) {
  // Set properties of the assembly language.
  // For example, the comment string.
  CommentString = "#";
  PrivateGlobalPrefix = ".L"; // Prefix for private global symbols
}
