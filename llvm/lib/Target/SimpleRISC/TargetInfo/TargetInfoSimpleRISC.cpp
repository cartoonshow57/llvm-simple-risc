#include "llvm/MC/TargetRegistry.h"
#include "TargetInfoSimpleRISC.h"

using namespace llvm;

extern "C" void LLVMInitializeSimpleRISCTargetInfo() {
  RegisterTarget<Triple::simplerisc, /*HasJIT=*/false> X(
      getTheSimpleRISCTarget(), "simplerisc", "SimpleRISC", "SimpleRISC");
}
