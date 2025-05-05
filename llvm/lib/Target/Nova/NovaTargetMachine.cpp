//@s nova-tm-cpp removed
// #include "llvm/MC/TargetRegistry.h"
// #include "llvm/Support/Compiler.h"
// #include "llvm/TargetParser/Triple.h"

// using namespace llvm;

// static llvm::Target &getTheNovaTarget() {
//     static Target TheNovaTarget;
//     return TheNovaTarget;
// }

// extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeNovaTargetInfo() {
//     llvm::RegisterTarget<llvm::Triple::mipsnova> X(::getTheNovaTarget(), "mipsnova", "MipsNova (32-bit big endian)", "Nova");
// };

// extern "C" void LLVMInitializeNovaTargetMC() {
//     // TODO: Add initialize target MC
// }

// extern "C" void LLVMInitializeNovaTarget() {
//     // TODO: Add initialize target
// }
//- nova-tm-cpp

//@s deleted-mc-init delete
// extern "C" void LLVMInitializeNovaTargetMC() {
//     // TODO: Add initialize target MC
// }
//- deleted-mc-init

//@s old-get-the-nova removed
// static Target &getTheNovaTarget() {
//- old-get-the-nova


//@s nova-target-info-tm
#include "NovaTargetInfo.h"
//- nova-target-info-tm
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Compiler.h"
#include "llvm/TargetParser/Triple.h"

using namespace llvm;

//@s new-get-the-nova replace=old-get-the-nova
Target &llvm::getTheNovaTarget() {
  //- new-get-the-nova
  static Target TheNovaTarget;
  return TheNovaTarget;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeNovaTargetInfo() {
    llvm::RegisterTarget<llvm::Triple::mipsnova> X(::getTheNovaTarget(), "mipsnova", "MipsNova (32-bit big endian)", "Nova");
};

extern "C" void LLVMInitializeNovaTarget() {
    // TODO: Add initialize target
}
