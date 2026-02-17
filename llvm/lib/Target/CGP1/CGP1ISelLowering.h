//@s cgp1-isel-lowering-1 end
//==-- CGP1 DAG Lowering Interface --------//

#ifndef LLVM_LIB_TARGET_CGP1_CGP1ISELLOWERING_H
#define LLVM_LIB_TARGET_CGP1_CGP1ISELLOWERING_H

#include "llvm/CodeGen/ISDOpcodes.h"
#include "llvm/CodeGen/TargetLowering.h"
namespace llvm {

namespace CGP1ISD {
enum NodeType : unsigned {
  FIRST_NUMBER = ISD::BUILTIN_OP_END,

  // Return
  Ret,

  // Function call
  Call,

  // Wrapper for global addresses
  Wrapper,

  // Hi/Lo for address materialization
  Hi,
  Lo,
  
  // Graphics operations
  DrawTriangle,
  DrawLine,
  ClearScreen,
  
  // GPU synchronization
  Barrier,
  
  // Kernel termination (STOP_CORE)
  EndKernel,

  // SIMT EXEC mask stack operations
  ExecPush,
  ExecPop,
  // SIMT reconvergence point (restore EXEC mask from stack)
  ExecReconverge,
  
  // Conditional branches
  BranchZero,     // Branch if register == 0
  BranchNotZero,  // Branch if register != 0
  BranchLTZ,      // Branch if register < 0
  BranchGEZ,      // Branch if register >= 0
  
  // SIMT branch with fall-through handling
  // Operands: Chain, ConditionMask, TakenExec, FallThroughExec, TakenDest, FallThroughDest
  // Automatically handles EXEC mask for both paths
  SIMTBranch,
  
  // Float immediate wrapper (prevents DAG folding)
  FPImm,

  // Atomic operations
  AtomicAdd,
  AtomicCas,
  AtomicMax,
  AtomicMin,
  AtomicExch,

  // Shared memory operations
  SharedLoad,
  SharedStore,

  // Warp operations
  WarpSize,
  LaneId,
  WarpAll,
  WarpAny,
  WarpBallot,
  WarpShuffle,
};
} // end namespace CGP1ISD
//- cgp1-isel-lowering-1

//@s cgp1-isel-lowering-2
class CGP1Subtarget;
class Function;

class CGP1TargetLowering : public TargetLowering {
public:
  explicit CGP1TargetLowering(const TargetMachine &TM,
                              const CGP1Subtarget &STI);

  SDValue LowerReturn(SDValue Chain, CallingConv::ID CallConv, bool isVarArg,
                      const SmallVectorImpl<ISD::OutputArg> &Outs,
                      const SmallVectorImpl<SDValue> &OutVals, const SDLoc &dl,
                      SelectionDAG &DAG) const override;

  SDValue LowerCall(TargetLowering::CallLoweringInfo &CLI,
                    SmallVectorImpl<SDValue> &InVals) const override;

  bool CanLowerReturn(CallingConv::ID CallConv, MachineFunction &MF,
                      bool IsVarArg,
                      const SmallVectorImpl<ISD::OutputArg> &Outs,
                      LLVMContext &Context, const Type *RetTy) const override;
  SDValue
  LowerFormalArguments(SDValue Chain, CallingConv::ID CallConv,
                       bool isVarArg,
                       const SmallVectorImpl<ISD::InputArg> &Ins,
                       const SDLoc &dl, SelectionDAG &DAG,
                       SmallVectorImpl<SDValue> &InVals) const override;
  SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const override;
  /// getTargetNodeName - This method returns the name of a target specific
  //  DAG node.
  const char *getTargetNodeName(unsigned Opcode) const override;

private:
  bool isKernelFunction(const Function &F) const;
  SDValue lowerIntrinsic(SDValue Op, SelectionDAG &DAG) const;
  SDValue lowerVoidIntrinsic(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerGlobalAddress(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerExternalSymbol(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerBR_CC(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerFrameIndex(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerSETCC(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerSELECT(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerConstantPool(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerConstantFP(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerFP_TO_SINT(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerSINT_TO_FP(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerROTL(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerROTR(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerSIGN_EXTEND(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerZERO_EXTEND(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerANY_EXTEND(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerTRUNCATE(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerSIGN_EXTEND_INREG(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerFMA(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerFCOPYSIGN(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerFRINT(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerFNEARBYINT(SDValue Op, SelectionDAG &DAG) const;
};

} // namespace llvm

#endif

//- cgp1-isel-lowering-2
