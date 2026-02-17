//@s cgp1-h-old commented
// /// This is for global functions in the CGP1 target.
// #ifndef LLVM_LIB_TARGET_CGP1_CGP1_H
// #define LLVM_LIB_TARGET_CGP1_CGP1_H
// #include "MCTargetDesc/CGP1MCTargetDesc.h"

// #endif
//- cgp1-h-old


/// This is for global functions in the CGP1 target.
#ifndef LLVM_LIB_TARGET_CGP1_CGP1_H
#define LLVM_LIB_TARGET_CGP1_CGP1_H

#include "MCTargetDesc/CGP1MCTargetDesc.h"
//@s cgp1-isel-dag-register end
#include "llvm/Pass.h"
#include "llvm/Support/CodeGen.h"
//- cgp1-isel-dag-register

//@s cgp1-isel-dag-register-1
namespace llvm {
  class FunctionPass;
  class CGP1TargetMachine;

  FunctionPass *createCGP1ISelDagLegacy(CGP1TargetMachine &TM,
                                     CodeGenOptLevel OptLevel);

  void initializeCGP1DAGToDAGISelLegacyPass(PassRegistry &);
  
  FunctionPass *createCGP1SIMTReconvergencePass();
  void initializeCGP1SIMTReconvergencePass(PassRegistry &);
} // namespace llvm
//- cgp1-isel-dag-register-1
#endif