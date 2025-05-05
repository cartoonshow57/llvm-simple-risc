//@s nova-target-info
#ifndef LLVM_LIB_TARGET_NOVA_TARGETINFO_H
#define LLVM_LIB_TARGET_NOVA_TARGETINFO_H

namespace llvm {
class Target;

Target &getTheNovaTarget();

} // namespace llvm

#endif

//- nova-target-info