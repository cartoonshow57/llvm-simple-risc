//@s cgp1-asm-printer.cpp
#include "CGP1.h"
#include "CGP1Subtarget.h"
#include "CGP1TargetMachine.h"
#include "TargetInfo/CGP1TargetInfo.h"
#include "MCTargetDesc/CGP1MCInstPrinter.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Debug.h"

#define DEBUG_TYPE "cgp1-asm-printer"

using namespace llvm;

namespace {
class CGP1AsmPrinter : public AsmPrinter {
public:
  CGP1AsmPrinter(TargetMachine &TM, std::unique_ptr<MCStreamer> Streamer)
  : AsmPrinter(TM, std::move(Streamer)) {}

  StringRef getPassName() const override {
    return "CGP1 Assembly Printer";
  }

  void emitInstruction(const MachineInstr *MI) override;

  // Lower the MachineInstr to MCInst
  void lowerInstruction(const MachineInstr &MI, MCInst &Inst);

  // bool lowerPseudoInstExpansion(const MachineInstr *MI, MCInst &Inst);
private:

  MCOperand lowerSymbolOperand(const MachineOperand &MO, MCSymbol *Sym);
};

MCOperand CGP1AsmPrinter::lowerSymbolOperand(const MachineOperand &MO, MCSymbol *Sym) {
  auto &Ctx = OutContext;
  MCSymbolRefExpr::VariantKind Kind = MCSymbolRefExpr::VK_None;
  
  // Handle target flags for %hi/%lo etc.
  unsigned TargetFlags = MO.getTargetFlags();
  // TODO: Add handling for hi/lo variants if needed
  
  const MCExpr *Expr = MCSymbolRefExpr::create(Sym, Kind, Ctx);
  
  // Add offset if present
  if (!MO.isJTI() && !MO.isMBB() && MO.getOffset()) {
    Expr = MCBinaryExpr::createAdd(Expr,
             MCConstantExpr::create(MO.getOffset(), Ctx), Ctx);
  }
  
  return MCOperand::createExpr(Expr);
}


void CGP1AsmPrinter::lowerInstruction(const MachineInstr &MI, MCInst &Inst) {
  // This function should convert the MachineInstr to MCInst
  // The implementation will depend on the specific instruction set
  // and how you want to represent it in the MCInst format.
  
  Inst.setOpcode(MI.getOpcode());
  for (const auto &Op : MI.operands()) {
    MCOperand MCOp;
    switch (Op.getType()) {
      case MachineOperand::MO_Register:
        // Skip implicit def/use and tied operands
        if (Op.isImplicit())
          continue;
        MCOp = MCOperand::createReg(Op.getReg());
        break;
      case MachineOperand::MO_Immediate:
        MCOp = MCOperand::createImm(Op.getImm());
        break;
      case MachineOperand::MO_MachineBasicBlock:
        MCOp = lowerSymbolOperand(Op, Op.getMBB()->getSymbol());
        break;
      case MachineOperand::MO_GlobalAddress:
        MCOp = lowerSymbolOperand(Op, getSymbol(Op.getGlobal()));
        break;
      case MachineOperand::MO_ExternalSymbol:
        MCOp = lowerSymbolOperand(Op, GetExternalSymbolSymbol(Op.getSymbolName()));
        break;
      case MachineOperand::MO_BlockAddress:
        MCOp = lowerSymbolOperand(Op, GetBlockAddressSymbol(Op.getBlockAddress()));
        break;
      case MachineOperand::MO_ConstantPoolIndex:
        MCOp = lowerSymbolOperand(Op, GetCPISymbol(Op.getIndex()));
        break;
      case MachineOperand::MO_RegisterMask:
        // Register masks are used for call clobbers, skip them in MCInst
        continue;
      case MachineOperand::MO_FrameIndex:
        // Frame indices should have been eliminated before this point
        // If we hit one, it might be a pseudo instruction
        continue;
      default:
        LLVM_DEBUG(dbgs() << "Unsupported operand type: " << Op.getType() << "\n");
        llvm_unreachable("Unsupported operand type");
    }
    Inst.addOperand(MCOp);
  }
}

} // end anonymous namespace

void CGP1AsmPrinter::emitInstruction(const MachineInstr *MI) {
  // Lower the instruction to MCInst
  MCInst Inst;
  lowerInstruction(*MI, Inst);
  EmitToStreamer(*OutStreamer, Inst);
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeCGP1AsmPrinter() {
  RegisterAsmPrinter<CGP1AsmPrinter> X(getTheCGP1Target());
}
//- cgp1-asm-printer.cpp
