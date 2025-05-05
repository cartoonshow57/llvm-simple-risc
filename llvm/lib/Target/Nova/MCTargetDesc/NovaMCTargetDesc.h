//@s nova-mc-tdesc-1
/// This file provides Nova-specific target descriptions.
#ifndef LLVM_LIB_TARGET_NOVA_MCTARGETDESC_H
#define LLVM_LIB_TARGET_NOVA_MCTARGETDESC_H

// Include symbolic names for registers. This includes the enum
// for register to register number mapping. (Nova::RA etc) and
// the register classes.
#define GET_REGINFO_ENUM
#include "NovaGenRegisterInfo.inc"

#endif
//- nova-mc-tdesc-1
