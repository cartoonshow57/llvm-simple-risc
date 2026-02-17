//@s create-reginfo commented
// #include "CGP1MCTargetDesc.h"
// #include "CGP1TargetInfo.h"

// #include "llvm/MC/MCRegisterInfo.h"
// #include "llvm/MC/TargetRegistry.h"

// using namespace llvm;

// #define GET_REGINFO_MC_DESC
// // Defines the InitCGP1MCRegisterInfo function
// #include "CGP1GenRegisterInfo.inc"

// static MCRegisterInfo* createCGP1MCRegisterInfo(const Triple &TT) {
//     MCRegisterInfo *X = new MCRegisterInfo();
//     InitCGP1MCRegisterInfo(X, CGP1::R14);
//     return X;
// }

// extern "C" void LLVMInitializeCGP1TargetMC() {
//   Target *T =  &getTheCGP1Target();
//   TargetRegistry::RegisterMCRegInfo(*T, createCGP1MCRegisterInfo);
// }
//- create-reginfo


#include "CGP1MCTargetDesc.h"
#include "TargetInfo/CGP1TargetInfo.h"
//@s include-mcsubtarget-info end
#include "llvm/MC/MCSubtargetInfo.h"
//- include-mcsubtarget-info
//@s include-mc-instrinfo end
#include "llvm/MC/MCInstrInfo.h"
//- include-mc-instrinfo
//@s include-mc-asm-info end 
#include "MCTargetDesc/CGP1MCAsmInfo.h"
#include "llvm/MC/MCDwarf.h"
//- include-mc-asm-info
//@s include-mc-instprinter
#include "MCTargetDesc/CGP1MCInstPrinter.h"
//- include-mc-instprinter
//@s include-mc-code-emitter
#include "MCTargetDesc/CGP1MCCodeEmitter.h"
//- include-mc-code-emitter
//@s include-mc-asm-backend
#include "MCTargetDesc/CGP1AsmBackend.h"
//- include-mc-asm-backend

#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

//@s cgp1-get-instrinfo
#define GET_INSTRINFO_MC_DESC
#define ENABLE_INSTR_PREDICATE_VERIFIER
#include "CGP1GenInstrInfo.inc"
//- cgp1-get-instrinfo

#define GET_REGINFO_MC_DESC
// Defines the InitCGP1MCRegisterInfo function
#include "CGP1GenRegisterInfo.inc"

//@s cgp1-get-subtargetinfo
#define GET_SUBTARGETINFO_MC_DESC
#include "CGP1GenSubtargetInfo.inc"
//- cgp1-get-subtargetinfo

static MCRegisterInfo* createCGP1MCRegisterInfo(const Triple &TT) {
    MCRegisterInfo *X = new MCRegisterInfo();
    InitCGP1MCRegisterInfo(X, CGP1::R14);
    return X;
}

//@s cgp1-create-subtarget end
static MCSubtargetInfo* createCGP1SubtargetInfo(const Triple &TT, StringRef CPU, StringRef FS) {
  if (CPU.empty())
      CPU = "generic";
  return createCGP1MCSubtargetInfoImpl(TT, CPU, CPU, FS);
}
//- cgp1-create-subtarget

//@s cgp1-create-asm-info end
static MCAsmInfo* createCGP1MCAsmInfo(const MCRegisterInfo &MRI, const Triple &TT, const MCTargetOptions &Options) {
  MCAsmInfo *X = new CGP1MCAsmInfo(TT);
  unsigned SP = MRI.getDwarfRegNum(CGP1::R13, true);  // R13 = Stack Pointer
  MCCFIInstruction Inst = MCCFIInstruction::createDefCfaRegister(nullptr, SP);
  X->addInitialFrameState(Inst);
  return X;
}
//- cgp1-create-asm-info

//@s register-instrinfo
static MCInstrInfo* createCGP1MCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitCGP1MCInstrInfo(X);
  return X;
}
//- register-instrinfo

//@s cgp1-create-instprinter
static MCInstPrinter* createCGP1MCInstPrinter(const Triple &T, unsigned SyntaxVariant, const MCAsmInfo &MAI, const MCInstrInfo &MII, const MCRegisterInfo &MRI) {
  return new CGP1InstPrinter(MAI, MII, MRI);
}
//- cgp1-create-instprinter
//@s cgp1-create-asm-backend
static MCAsmBackend* createCGP1AsmBackend(const Target &T, const MCSubtargetInfo &STI,
                                          const MCRegisterInfo &MRI,
                                          const MCTargetOptions &Options) {
  return new CGP1AsmBackend(T, MRI, STI.getTargetTriple());
}
//- cgp1-create-asm-backend

extern "C" void LLVMInitializeCGP1TargetMC() {
  Target *T =  &getTheCGP1Target();
  TargetRegistry::RegisterMCRegInfo(*T, createCGP1MCRegisterInfo);
  //@s cgp1-register-subtarget end
  TargetRegistry::RegisterMCSubtargetInfo(*T, createCGP1SubtargetInfo);
  //- cgp1-register-subtarget
  //@s register-instrinfo-2 end
  TargetRegistry::RegisterMCInstrInfo(*T, createCGP1MCInstrInfo);
  //- register-instrinfo-2
  //@s register-asm-info end
  TargetRegistry::RegisterMCAsmInfo(*T, createCGP1MCAsmInfo);
  //- register-asm-info
  //@s register-inst-printer
  TargetRegistry::RegisterMCInstPrinter(*T, createCGP1MCInstPrinter);
  //- register-inst-printer
  //@s register-code-emitter
  TargetRegistry::RegisterMCCodeEmitter(*T, createCGP1MCCodeEmitter);
  //- register-code-emitter
  //@s register-asm-backend
  TargetRegistry::RegisterMCAsmBackend(*T, createCGP1AsmBackend);
  //- register-asm-backend
}
