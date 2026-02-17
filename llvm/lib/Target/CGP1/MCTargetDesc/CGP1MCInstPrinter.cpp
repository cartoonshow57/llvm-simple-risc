//@s cgp1-inst-printer-1 end
#include "CGP1MCInstPrinter.h"
#include "CGP1InstrInfo.h"
#include "llvm/MC/MCInst.h"
#define DEBUG_TYPE "cgp1-mcinst-printer"

using namespace llvm;

#define PRINT_ALIAS_INSTR
#include "CGP1GenAsmWriter.inc"
//- cgp1-inst-printer-1

//@s cgp1-inst-printer-2
void CGP1InstPrinter::printInst(const MCInst *MI, uint64_t Address, StringRef Annot,
  const MCSubtargetInfo &STI, raw_ostream &O) {
    // check if we have an alias
    if (!printAliasInstr(MI, Address, O)) {
      printInstruction(MI, Address, O);
    }
    printAnnotation(O, Annot);
}
//- cgp1-inst-printer-2

//@s cgp1-inst-printer-3 end
void CGP1InstPrinter::printRegName(raw_ostream &OS, MCRegister Reg) {
  OS << "$" << StringRef(getRegisterName(Reg)).lower();
}
//- cgp1-inst-printer-3

//@s cgp1-inst-printer-4
void CGP1InstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                   raw_ostream &O) {
  const MCOperand &Op = MI->getOperand(OpNo);
  if (Op.isReg()) {
    printRegName(O, Op.getReg());
    return;
  }

  if (Op.isImm()) {
    O << Op.getImm();
    return;
  }

  assert(Op.isExpr() && "unknown operand type");
  Op.getExpr()->print(O, &MAI, true);
}
//- cgp1-inst-printer-4

//@s cgp1-inst-printer-5
// Print memory operand in CGP1 ISA style: [base + offset]
void CGP1InstPrinter::printMemOperand(const MCInst *MI, unsigned OpNo,
                                      raw_ostream &O) {
  // The memory operand consists of two operands: base register and offset
  const MCOperand &BaseOp = MI->getOperand(OpNo);
  const MCOperand &OffsetOp = MI->getOperand(OpNo + 1);

  // Print in [base + offset] format to match hardware ISA
  O << "[";
  if (BaseOp.isReg()) {
    printRegName(O, BaseOp.getReg());
  }
  
  // Print offset
  if (OffsetOp.isImm()) {
    int64_t Offset = OffsetOp.getImm();
    if (Offset != 0) {
      if (Offset > 0)
        O << " + " << Offset;
      else
        O << " - " << -Offset;
    }
  } else if (OffsetOp.isExpr()) {
    O << " + ";
    OffsetOp.getExpr()->print(O, &MAI, true);
  }
  O << "]";
}
//- cgp1-inst-printer-5
