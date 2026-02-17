//@s cgp1-mc-tdesc-1 commented
// /// This file provides CGP1-specific target descriptions.
// #ifndef LLVM_LIB_TARGET_CGP1_MCTARGETDESC_H
// #define LLVM_LIB_TARGET_CGP1_MCTARGETDESC_H

// // Include symbolic names for registers. This includes the enum
// // for register to register number mapping. (CGP1::RA etc) and
// // the register classes.
// #define GET_REGINFO_ENUM
// #include "CGP1GenRegisterInfo.inc"

// #endif
//- cgp1-mc-tdesc-1


/// This file provides CGP1-specific target descriptions.
#ifndef LLVM_LIB_TARGET_CGP1_MCTARGETDESC_H
#define LLVM_LIB_TARGET_CGP1_MCTARGETDESC_H

// Include symbolic names for registers. This includes the enum
// for register to register number mapping. (CGP1::RA etc) and
// the register classes.
#define GET_REGINFO_ENUM
#include "CGP1GenRegisterInfo.inc"

//@s cgp1-get-subtargetinfo-h end
#define GET_SUBTARGETINFO_ENUM
#include "CGP1GenSubtargetInfo.inc"
//- cgp1-get-subtargetinfo-h

//@s cgp1-get-instrinfo-h
#define GET_INSTRINFO_ENUM
#include "CGP1GenInstrInfo.inc"
//- cgp1-get-instrinfo-h

namespace llvm {
class MCCodeEmitter;
class MCContext;
class MCInstrInfo;
class MCObjectTargetWriter;
class Triple;

MCCodeEmitter *createCGP1MCCodeEmitter(const MCInstrInfo &MCII,
                                        MCContext &Ctx);
MCObjectTargetWriter *createCGP1ELFObjectWriter(const Triple &TT);
} // end namespace llvm

#endif