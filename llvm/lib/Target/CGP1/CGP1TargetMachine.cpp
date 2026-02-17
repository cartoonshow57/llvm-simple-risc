//@s cgp1-tm-cpp removed
// #include "llvm/MC/TargetRegistry.h"
// #include "llvm/Support/Compiler.h"
// #include "llvm/TargetParser/Triple.h"

// using namespace llvm;

// static llvm::Target &getTheCGP1Target() {
//     static Target TheCGP1Target;
//     return TheCGP1Target;
// }

// extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeCGP1TargetInfo() {
//     llvm::RegisterTarget<llvm::Triple::cgp1> X(::getTheCGP1Target(),
//     "cgp1", "CGP1 (32-bit big endian)", "CGP1");
// };

// extern "C" void LLVMInitializeCGP1TargetMC() {
//     // TODO: Add initialize target MC
// }

// extern "C" void LLVMInitializeCGP1Target() {
//     // TODO: Add initialize target
// }
//- cgp1-tm-cpp


//@s deleted-mc-init delete
// extern "C" void LLVMInitializeCGP1TargetMC() {
//     // TODO: Add initialize target MC
// }
//- deleted-mc-init

//@s old-get-the-cgp1 removed
// static Target &getTheCGP1Target() {
//- old-get-the-cgp1

//@s old-initialize-cgp1-target removed
//   // TODO: Add initialize target
//- old-initialize-cgp1-target

#include "CGP1.h"
#include "CGP1TargetMachine.h"
#include "CGP1SIMTReconvergence.h"
#include "TargetInfo/CGP1TargetInfo.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/PassRegistry.h"
#include "llvm/Transforms/Scalar.h"

using namespace llvm;

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeCGP1Target() {
  RegisterTargetMachine<CGP1TargetMachine> X(getTheCGP1Target());
  PassRegistry &PR = *PassRegistry::getPassRegistry();
  initializeCGP1DAGToDAGISelLegacyPass(PR);
  initializeCGP1SIMTReconvergencePass(PR);
}

//@s cgp1-pass-config
namespace {
class CGP1PassConfig : public TargetPassConfig {
public:
  CGP1PassConfig(CGP1TargetMachine &TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM) {}

  CGP1TargetMachine &getCGP1TargetMachine() const {
    return getTM<CGP1TargetMachine>();
  }
  bool addPreISel() override {
    addPass(createStructurizeCFGPass(false));
    return false;
  }
  bool addInstSelector() override {
    addPass(createCGP1ISelDagLegacy(getCGP1TargetMachine(), getOptLevel()));
    return false;
  }
  void addPreEmitPass() override {
    // Add SIMT reconvergence pass to handle automatic EXEC mask management
    addPass(createCGP1SIMTReconvergencePass());
  }
};
} // namespace

TargetPassConfig *CGP1TargetMachine::createPassConfig(PassManagerBase &PM) {
  return new CGP1PassConfig(*this, PM);
}
//- cgp1-pass-config
