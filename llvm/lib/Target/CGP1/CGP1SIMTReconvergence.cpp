//===-- CGP1SIMTReconvergence.cpp - SIMT Reconvergence Pass --------------===//
//
// This pass handles automatic SIMT reconvergence by:
// 1. Detecting reconvergence points (basic blocks with multiple predecessors)
// 2. Inserting EXEC mask restoration code at reconvergence points
// 3. Handling fall-through paths by updating EXEC mask
//
//===----------------------------------------------------------------------===//

#include "CGP1.h"
#include "CGP1InstrInfo.h"
#include "CGP1Subtarget.h"
#include "MCTargetDesc/CGP1MCTargetDesc.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/CodeGen/Passes.h"

using namespace llvm;

#define DEBUG_TYPE "cgp1-simt-reconverge"

namespace {
class CGP1SIMTReconvergence : public MachineFunctionPass {
public:
  static char ID;
  CGP1SIMTReconvergence() : MachineFunctionPass(ID) {}

  bool runOnMachineFunction(MachineFunction &MF) override;
  StringRef getPassName() const override {
    return "CGP1 SIMT Reconvergence Pass";
  }

private:
  bool isKernelFunction(const Function &F) const;
  void handleReconvergencePoint(MachineFunction &MF, MachineBasicBlock *MBB);
  void handleFallThroughPath(MachineFunction &MF, MachineBasicBlock *MBB);
};
} // namespace

char CGP1SIMTReconvergence::ID = 0;

bool CGP1SIMTReconvergence::isKernelFunction(const Function &F) const {
  if (F.getCallingConv() == CallingConv::SPIR_KERNEL)
    return true;
  if (F.hasFnAttribute("cgp1_kernel"))
    return true;
  return false;
}

void CGP1SIMTReconvergence::handleReconvergencePoint(MachineFunction &MF,
                                                     MachineBasicBlock *MBB) {
  // A reconvergence point is a basic block with multiple predecessors
  // At reconvergence, we need to restore the EXEC mask from the stack
  
  if (MBB->pred_size() < 2)
    return; // Not a reconvergence point
  
  const TargetInstrInfo *TII = MF.getSubtarget().getInstrInfo();
  DebugLoc DL;
  
  // Find the first non-debug instruction
  MachineBasicBlock::iterator MBBI = MBB->begin();
  while (MBBI != MBB->end() && MBBI->isDebugInstr())
    ++MBBI;
  
  if (MBBI != MBB->end())
    DL = MBBI->getDebugLoc();
  
  // Pop EXEC mask from stack and restore to R15
  // POP instruction: pop R15 (restores EXEC mask)
  BuildMI(*MBB, MBBI, DL, TII->get(CGP1::POP), CGP1::R15);
}

void CGP1SIMTReconvergence::handleFallThroughPath(MachineFunction &MF,
                                                   MachineBasicBlock *MBB) {
  // Handle fall-through path: if this block is a fall-through from a SIMT branch,
  // we need to update EXEC mask for threads that didn't take the branch
  
  // Check if this block has a single predecessor that ends with SIMT_BRANCH
  if (MBB->pred_size() != 1)
    return;
  
  MachineBasicBlock *Pred = *MBB->pred_begin();
  if (Pred->empty())
    return;
  
  // Check if predecessor ends with SIMT_BRANCH
  MachineBasicBlock::iterator LastMI = Pred->getLastNonDebugInstr();
  if (LastMI == Pred->end())
    return;
  
  // Check if it's a SIMT_BRANCH instruction
  if (LastMI->getOpcode() != CGP1::SIMT_BRANCH)
    return;
  
  // Check if this MBB is the fall-through (not the taken destination)
  // The taken destination is in the SIMT_BRANCH instruction
  MachineBasicBlock *TakenDest = LastMI->getOperand(4).getMBB();
  if (TakenDest == MBB)
    return; // This is the taken path, not fall-through
  
  // This is the fall-through path
  // We need to update EXEC mask: EXEC = FallThroughExec (from SIMT_BRANCH)
  // The FallThroughExec is stored in operand 3 of SIMT_BRANCH
  const TargetInstrInfo *TII = MF.getSubtarget().getInstrInfo();
  DebugLoc DL;
  
  MachineBasicBlock::iterator MBBI = MBB->begin();
  while (MBBI != MBB->end() && MBBI->isDebugInstr())
    ++MBBI;
  
  if (MBBI != MBB->end())
    DL = MBBI->getDebugLoc();
  
  // Get the FallThroughExec from the SIMT_BRANCH instruction
  // Operand 3 is FallThroughExec (register)
  Register FallThroughExecReg = LastMI->getOperand(3).getReg();
  
  // Update EXEC mask (R15) with FallThroughExec
  BuildMI(*MBB, MBBI, DL, TII->get(CGP1::COPY_REG), CGP1::R15)
      .addReg(FallThroughExecReg);
}

bool CGP1SIMTReconvergence::runOnMachineFunction(MachineFunction &MF) {
  const Function &F = MF.getFunction();
  
  // Only process kernel functions (SIMT context)
  if (!isKernelFunction(F))
    return false;
  
  bool Changed = false;
  
  // Find all reconvergence points (basic blocks with multiple predecessors)
  for (MachineBasicBlock &MBB : MF) {
    if (MBB.pred_size() >= 2) {
      // This is a reconvergence point
      handleReconvergencePoint(MF, &MBB);
      Changed = true;
    }
    
    // Handle fall-through paths
    handleFallThroughPath(MF, &MBB);
  }
  
  return Changed;
}

FunctionPass *llvm::createCGP1SIMTReconvergencePass() {
  return new CGP1SIMTReconvergence();
}

INITIALIZE_PASS_BEGIN(CGP1SIMTReconvergence, DEBUG_TYPE,
                      "CGP1 SIMT Reconvergence Pass", false, false)
INITIALIZE_PASS_END(CGP1SIMTReconvergence, DEBUG_TYPE,
                    "CGP1 SIMT Reconvergence Pass", false, false)

