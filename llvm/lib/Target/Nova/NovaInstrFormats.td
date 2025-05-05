//@s nova-instr-formats-1 end
//===-- NovaInstrFormats.td - Nova Instruction Formats --------------------===//
// This file contains the instruction formats for the Nova architecture.
//===----------------------------------------------------------------------===//
//- nova-instr-formats-1

//@s nova-instr-formats-2 end
class NovaInst<dag outs, dag ins, string asmString> : Instruction {
  let Namespace = "Nova";
  let OutOperandList = outs;
  let InOperandList = ins;
  let AsmString = asmString;
}
//- nova-instr-formats-2

//@s nova-instr-formats-3
class PseudoNovaInst<dag outs, dag ins, string asmString> : NovaInst<outs, ins, asmString> {
  let isPseudo = 1;
  let isCodeGenOnly = 1;
}
//- nova-instr-formats-3
