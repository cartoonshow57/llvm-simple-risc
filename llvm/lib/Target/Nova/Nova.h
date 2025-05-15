//@s nova-h-old commented
// /// This is for global functions in the Nova target.
// #ifndef LLVM_LIB_TARGET_NOVA_NOVA_H
// #define LLVM_LIB_TARGET_NOVA_NOVA_H
// #include "MCTargetDesc/NovaMCTargetDesc.h"

// #endif
//- nova-h-old


/// This is for global functions in the Nova target.
#ifndef LLVM_LIB_TARGET_NOVA_NOVA_H
#define LLVM_LIB_TARGET_NOVA_NOVA_H

#include "MCTargetDesc/NovaMCTargetDesc.h"
//@s nova-isel-dag-register end
#include "llvm/Pass.h"
#include "llvm/Support/CodeGen.h"
//- nova-isel-dag-register

//@s nova-isel-dag-register-1
namespace llvm {
  class FunctionPass;
  class NovaTargetMachine;

  FunctionPass *createNovaISelDagLegacy(NovaTargetMachine &TM,
                                     CodeGenOptLevel OptLevel);

  void initializeNovaDAGToDAGISelLegacyPass(PassRegistry &);
} // namespace llvm
//- nova-isel-dag-register-1
#endif