//@s create-reginfo
#include "NovaMCTargetDesc.h"
#include "NovaTargetInfo.h"

#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

#define GET_REGINFO_MC_DESC
// Defines the InitNovaMCRegisterInfo function
#include "NovaGenRegisterInfo.inc"

static MCRegisterInfo* createNovaMCRegisterInfo(const Triple &TT) {
    MCRegisterInfo *X = new MCRegisterInfo();
    InitNovaMCRegisterInfo(X, Nova::RA);
    return X;
}

extern "C" void LLVMInitializeNovaTargetMC() {
  Target *T =  &getTheNovaTarget();
  TargetRegistry::RegisterMCRegInfo(*T, createNovaMCRegisterInfo);
}
//- create-reginfo