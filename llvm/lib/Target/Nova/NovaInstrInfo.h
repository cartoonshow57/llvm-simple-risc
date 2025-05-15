//@s nova-instr-info.h
#ifndef LLVM_LIB_TARGET_NOVA_NOVAINSTRINFO_H
#define LLVM_LIB_TARGET_NOVA_NOVAINSTRINFO_H

#include "Nova.h"
#include "NovaRegisterInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "NovaGenInstrInfo.inc"

namespace llvm {
class NovaSubtarget;

class NovaInstrInfo : public NovaGenInstrInfo {
public:
  explicit NovaInstrInfo(const NovaSubtarget &STI);
protected:
  const NovaSubtarget &Subtarget;
};
} // end namespace llvm

#endif
//- nova-instr-info.h