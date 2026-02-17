//@s cgp1-isel-dag-to-dag.h
#ifndef LLVM_LIB_TARGET_CGP1_CGP1ISELDAGTODAG_H
#define LLVM_LIB_TARGET_CGP1_CGP1ISELDAGTODAG_H

#include "CGP1Subtarget.h"
#include "CGP1TargetMachine.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/SelectionDAGISel.h"

namespace llvm {
class CGP1DAGToDAGISel final : public SelectionDAGISel {
  const CGP1Subtarget *Subtarget;

public:
  explicit CGP1DAGToDAGISel(CGP1TargetMachine &TM, CodeGenOptLevel OptLevel)
      : SelectionDAGISel(TM, OptLevel) {}

  bool runOnMachineFunction(MachineFunction &MF) override;

private:
#include "CGP1GenDAGISel.inc"

  void Select(SDNode *Node) override;

  // Complex pattern selector for memory addresses.
  // Used by load/store instructions to match base+offset addressing.
  bool SelectAddr(SDValue Addr, SDValue &Base, SDValue &Offset);
};
} // namespace llvm

#endif
//- cgp1-isel-dag-to-dag.h
