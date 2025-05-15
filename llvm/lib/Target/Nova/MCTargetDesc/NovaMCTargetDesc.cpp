//@s create-reginfo commented
// #include "NovaMCTargetDesc.h"
// #include "NovaTargetInfo.h"

// #include "llvm/MC/MCRegisterInfo.h"
// #include "llvm/MC/TargetRegistry.h"

// using namespace llvm;

// #define GET_REGINFO_MC_DESC
// // Defines the InitNovaMCRegisterInfo function
// #include "NovaGenRegisterInfo.inc"

// static MCRegisterInfo* createNovaMCRegisterInfo(const Triple &TT) {
//     MCRegisterInfo *X = new MCRegisterInfo();
//     InitNovaMCRegisterInfo(X, Nova::RA);
//     return X;
// }

// extern "C" void LLVMInitializeNovaTargetMC() {
//   Target *T =  &getTheNovaTarget();
//   TargetRegistry::RegisterMCRegInfo(*T, createNovaMCRegisterInfo);
// }
//- create-reginfo


#include "NovaMCTargetDesc.h"
#include "NovaTargetInfo.h"
//@s include-mcsubtarget-info end
#include "llvm/MC/MCSubtargetInfo.h"
//- include-mcsubtarget-info
//@s include-mc-instrinfo end
#include "llvm/MC/MCInstrInfo.h"
//- include-mc-instrinfo
//@s include-mc-asm-info end 
#include "MCTargetDesc/NovaMCAsmInfo.h"
#include "llvm/MC/MCDwarf.h"
//- include-mc-asm-info
//@s include-mc-instprinter
#include "MCTargetDesc/NovaMCInstPrinter.h"
//- include-mc-instprinter

#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

//@s nova-get-instrinfo
#define GET_INSTRINFO_MC_DESC
#define ENABLE_INSTR_PREDICATE_VERIFIER
#include "NovaGenInstrInfo.inc"
//- nova-get-instrinfo

#define GET_REGINFO_MC_DESC
// Defines the InitNovaMCRegisterInfo function
#include "NovaGenRegisterInfo.inc"

//@s nova-get-subtargetinfo
#define GET_SUBTARGETINFO_MC_DESC
#include "NovaGenSubtargetInfo.inc"
//- nova-get-subtargetinfo

static MCRegisterInfo* createNovaMCRegisterInfo(const Triple &TT) {
    MCRegisterInfo *X = new MCRegisterInfo();
    InitNovaMCRegisterInfo(X, Nova::RA);
    return X;
}

//@s nova-create-subtarget end
static MCSubtargetInfo* createNovaSubtargetInfo(const Triple &TT, StringRef CPU, StringRef FS) {
  if (CPU.empty())
      CPU = "generic";
  return createNovaMCSubtargetInfoImpl(TT, CPU, CPU, FS);
}
//- nova-create-subtarget

//@s nova-create-asm-info end
static MCAsmInfo* createNovaMCAsmInfo(const MCRegisterInfo &MRI, const Triple &TT, const MCTargetOptions &Options) {
  MCAsmInfo *X = new NovaMCAsmInfo(TT);
  unsigned SP = MRI.getDwarfRegNum(Nova::SP, true);
  MCCFIInstruction Inst = MCCFIInstruction::createDefCfaRegister(nullptr, SP);
  X->addInitialFrameState(Inst);
  return X;
}
//- nova-create-asm-info

//@s register-instrinfo
static MCInstrInfo* createNovaMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitNovaMCInstrInfo(X);
  return X;
}
//- register-instrinfo

//@s nova-create-instprinter
static MCInstPrinter* createNovaMCInstPrinter(const Triple &T, unsigned SyntaxVariant, const MCAsmInfo &MAI, const MCInstrInfo &MII, const MCRegisterInfo &MRI) {
  return new NovaInstPrinter(MAI, MII, MRI);
}
//- nova-create-instprinter

extern "C" void LLVMInitializeNovaTargetMC() {
  Target *T =  &getTheNovaTarget();
  TargetRegistry::RegisterMCRegInfo(*T, createNovaMCRegisterInfo);
  //@s nova-register-subtarget end
  TargetRegistry::RegisterMCSubtargetInfo(*T, createNovaSubtargetInfo);
  //- nova-register-subtarget
  //@s register-instrinfo-2 end
  TargetRegistry::RegisterMCInstrInfo(*T, createNovaMCInstrInfo);
  //- register-instrinfo-2
  //@s register-asm-info end
  TargetRegistry::RegisterMCAsmInfo(*T, createNovaMCAsmInfo);
  //- register-asm-info
  //@s register-inst-printer
  TargetRegistry::RegisterMCInstPrinter(*T, createNovaMCInstPrinter);
  //- register-inst-printer
}
