//@s nova-inst-printer-1 end
#include "NovaMCInstPrinter.h"
#include "NovaInstrInfo.h"
#include "llvm/MC/MCInst.h"
#define DEBUG_TYPE "nova-mcinst-printer"

using namespace llvm;

#define PRINT_ALIAS_INSTR
#include "NovaGenAsmWriter.inc"
//- nova-inst-printer-1

//@s nova-inst-printer-2
void NovaInstPrinter::printInst(const MCInst *MI, uint64_t Address, StringRef Annot,
  const MCSubtargetInfo &STI, raw_ostream &O) {
    // check if we have an alias
    if (!printAliasInstr(MI, Address, O)) {
      printInstruction(MI, Address, O);
    }
    printAnnotation(O, Annot);
}
//- nova-inst-printer-2

//@s nova-inst-printer-3 end
void NovaInstPrinter::printRegName(raw_ostream &OS, MCRegister Reg) {
  OS << "$" << StringRef(getRegisterName(Reg)).lower();
}
//- nova-inst-printer-3

//@s nova-inst-printer-4
void NovaInstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
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
//- nova-inst-printer-4
