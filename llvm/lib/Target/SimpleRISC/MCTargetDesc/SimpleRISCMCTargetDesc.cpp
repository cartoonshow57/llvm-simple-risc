#include "SimpleRISCMCAsmInfo.h"
#include "TargetInfo/TargetInfoSimpleRISC.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"

// The TableGen-generated files and all related functions must be
// enclosed in the llvm namespace.
namespace llvm {

#define GET_REGINFO_MC_DESC
#include "SimpleRISCGenRegisterInfo.inc"

#define GET_INSTRINFO_MC_DESC
#include "SimpleRISCGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "SimpleRISCGenSubtargetInfo.inc"

static MCInstrInfo *createSimpleRISCMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitSimpleRISCMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createSimpleRISCMCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  // The second argument is the Return Address Register. R0 is a placeholder.
  InitSimpleRISCMCRegisterInfo(X, SimpleRISC::R0);
  return X;
}

static MCSubtargetInfo *
createSimpleRISCMCSubtargetInfo(const Triple &TT, StringRef CPU, StringRef FS) {
  return createSimpleRISCMCSubtargetInfoImpl(TT, CPU, CPU, FS);
}

static MCAsmInfo *createSimpleRISCMCAsmInfo(const MCRegisterInfo &MRI,
                                           const Triple &TT,
                                           const MCTargetOptions &Options) {
  return new SimpleRISCMCAsmInfo(TT);
}

} // end namespace llvm

// The entry point for registering the MC components must be outside the namespace.
extern "C" void LLVMInitializeSimpleRISCTargetMC() {
  // Register the MC asm info.
  llvm::RegisterMCAsmInfoFn X(llvm::getTheSimpleRISCTarget(), llvm::createSimpleRISCMCAsmInfo);

  // Register the MC instruction info.
  llvm::TargetRegistry::RegisterMCInstrInfo(llvm::getTheSimpleRISCTarget(),
                                      llvm::createSimpleRISCMCInstrInfo);

  // Register the MC register info.
  llvm::TargetRegistry::RegisterMCRegInfo(llvm::getTheSimpleRISCTarget(),
                                    llvm::createSimpleRISCMCRegisterInfo);

  // Register the MC subtarget info.
  llvm::TargetRegistry::RegisterMCSubtargetInfo(llvm::getTheSimpleRISCTarget(),
                                          llvm::createSimpleRISCMCSubtargetInfo);
}
