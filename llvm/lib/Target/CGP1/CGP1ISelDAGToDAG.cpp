//@s cgp1-isel-dag.cpp-1 end
#include "CGP1ISelDAGToDAG.h"
#include "CGP1Subtarget.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/CodeGen/SelectionDAGNodes.h"
#include "llvm/Pass.h"
#include "llvm/Support/CodeGen.h"

using namespace llvm;

#define DEBUG_TYPE "cgp1-isel"

namespace {
class CGP1DAGToDAGISelLegacy : public SelectionDAGISelLegacy {
public:
  static char ID;
  CGP1DAGToDAGISelLegacy(CGP1TargetMachine &TM, CodeGenOptLevel OptLevel)
      : SelectionDAGISelLegacy(
            ID, std::make_unique<CGP1DAGToDAGISel>(TM, OptLevel)) {}
};
} // namespace

char CGP1DAGToDAGISelLegacy::ID = 0;

INITIALIZE_PASS(CGP1DAGToDAGISelLegacy, DEBUG_TYPE, "cgp1-isel", false, false);


FunctionPass *llvm::createCGP1ISelDagLegacy(CGP1TargetMachine &TM,
                                     CodeGenOptLevel OptLevel) {
  return new CGP1DAGToDAGISelLegacy(TM, OptLevel);
}

bool CGP1DAGToDAGISel::runOnMachineFunction(MachineFunction &MF) {
  Subtarget =
      &static_cast<const CGP1Subtarget &>(MF.getSubtarget<CGP1Subtarget>());
  return SelectionDAGISel::runOnMachineFunction(MF);
}

void CGP1DAGToDAGISel::Select(SDNode *Node) {
  switch (Node->getOpcode()) {
  case ISD::LOAD: {
    auto *LD = cast<LoadSDNode>(Node);
    if (LD->getAddressSpace() == 3) {
      SDLoc DL(Node);
      SDValue Chain = LD->getChain();
      SDValue Addr = LD->getBasePtr();
      ReplaceNode(Node,
                  CurDAG->getMachineNode(CGP1::SHARED_LOAD, DL,
                                         LD->getValueType(0), MVT::Other,
                                         Chain, Addr));
      return;
    }
    break;
  }
  case ISD::STORE: {
    auto *ST = cast<StoreSDNode>(Node);
    if (ST->getAddressSpace() == 3) {
      SDLoc DL(Node);
      SDValue Chain = ST->getChain();
      SDValue Addr = ST->getBasePtr();
      SDValue Val = ST->getValue();
      ReplaceNode(Node,
                  CurDAG->getMachineNode(CGP1::SHARED_STORE, DL, MVT::Other,
                                         Chain, Addr, Val));
      return;
    }
    break;
  }
  case CGP1ISD::ExecReconverge: {
    // Handle reconvergence: pop EXEC mask from stack and restore to R15
    SDLoc DL(Node);
    SDValue Chain = Node->getOperand(0);
    
    // Pop EXEC mask from stack
    SDValue Pop = CurDAG->getNode(CGP1ISD::ExecPop, DL,
                                   CurDAG->getVTList(MVT::i32, MVT::Other),
                                   Chain);
    
    // Restore to R15
    SDValue NewChain = CurDAG->getCopyToReg(Pop.getValue(1), DL, CGP1::R15, Pop);
    
    // Replace with a chain node
    ReplaceNode(Node, NewChain.getNode());
    return;
  }
  case CGP1ISD::SIMTBranch: {
    // Handle SIMT branch with both taken and fall-through paths
    // Operands: Chain, ConditionMask, TakenExec, FallThroughExec, TakenDest, FallThroughDest
    SDLoc DL(Node);
    SDValue Chain = Node->getOperand(0);
    SDValue ConditionMask = Node->getOperand(1);
    SDValue TakenExec = Node->getOperand(2);
    SDValue FallThroughExec = Node->getOperand(3);
    SDValue TakenDest = Node->getOperand(4);
    SDValue FallThroughDest = Node->getOperand(5);
    
    // For SIMT branches:
    // 1. Update EXEC mask for taken path and branch if needed
    // 2. Fall-through path EXEC mask will be handled by reconvergence pass
    
    // Update EXEC mask for taken path
    Chain = CurDAG->getCopyToReg(Chain, DL, CGP1::R15, TakenExec);
    
    // Check if any thread should take the branch
    SDValue Zero = CurDAG->getConstant(0, DL, MVT::i32);
    SDValue BranchCond = CurDAG->getNode(ISD::SETCC, DL, MVT::i32, TakenExec, Zero,
                                          CurDAG->getCondCode(ISD::SETNE));
    
    // Emit SIMT_BRANCH pseudo instruction with all operands
    // Operands: Chain, ConditionMask, TakenExec, FallThroughExec, TakenDest, FallThroughDest
    ReplaceNode(Node, CurDAG->getMachineNode(CGP1::SIMT_BRANCH, DL, MVT::Other,
                                             Chain, ConditionMask, TakenExec, 
                                             FallThroughExec, TakenDest, FallThroughDest));
    return;
  }
  default:
    break;
  }
  SelectCode(Node);
}

/// SelectAddr - Match address for load/store instructions.
/// This selects base + offset addressing mode.
/// Handles both stack (frame index) and global memory addresses.
bool CGP1DAGToDAGISel::SelectAddr(SDValue Addr, SDValue &Base,
                                  SDValue &Offset) {
  SDLoc DL(Addr);
  MVT ValTy = Addr.getSimpleValueType();

  // Case 1: If the address is a frame index, select it directly with offset 0.
  if (FrameIndexSDNode *FIN = dyn_cast<FrameIndexSDNode>(Addr)) {
    Base = CurDAG->getTargetFrameIndex(FIN->getIndex(), ValTy);
    Offset = CurDAG->getTargetConstant(0, DL, ValTy);
    return true;
  }


  // Case 2: If the address is add(base, const), select base + offset.
  if (Addr.getOpcode() == ISD::ADD) {
    SDValue Op0 = Addr.getOperand(0);
    SDValue Op1 = Addr.getOperand(1);
    
    // Check if second operand is a constant that fits in 16 bits
    if (ConstantSDNode *CN = dyn_cast<ConstantSDNode>(Op1)) {
      if (isInt<16>(CN->getSExtValue())) {
        // If first operand is a frame index
        if (FrameIndexSDNode *FIN = dyn_cast<FrameIndexSDNode>(Op0)) {
          Base = CurDAG->getTargetFrameIndex(FIN->getIndex(), ValTy);
          Offset = CurDAG->getTargetConstant(CN->getSExtValue(), DL, ValTy);
          return true;
        }
        // For general register + constant (global memory access)
        Base = Op0;
        Offset = CurDAG->getTargetConstant(CN->getSExtValue(), DL, ValTy);
        return true;
      }
    }
  }

  // Case 3: Default - treat address as base register with offset 0.
  // This handles global memory pointers (ptr addrspace(1) %ptr).
  Base = Addr;
  Offset = CurDAG->getTargetConstant(0, DL, ValTy);
  return true;
}
//- cgp1-isel-dag.cpp-1
