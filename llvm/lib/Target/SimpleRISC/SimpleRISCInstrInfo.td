//===-- SimpleRISCInstrInfo.td - Instructions -------------*- tablegen -*-===//
//
// This file describes the SimpleRISC instruction set.
//
//===----------------------------------------------------------------------===//

// A generic instruction format for our target.
class SimpleRISCInst<dag outs, dag ins, string asmstr, list<dag> pattern>
  : Instruction {
  let OutOperandList = outs;
  let InOperandList  = ins;
  let AsmString      = asmstr;
  let Pattern        = pattern;
}

// These pseudo-instructions are required by LLVM for stack management.
// Their patterns (callseq_start/end) are now visible via the master .td file.
def ADJCALLSTACKDOWN : SimpleRISCInst<(outs), (ins i32imm:$amt1, i32imm:$amt2),
                                     "", [(callseq_start timm:$amt1, timm:$amt2)]> {
  let isCodeGenOnly = 1;
}
def ADJCALLSTACKUP   : SimpleRISCInst<(outs), (ins i32imm:$amt1, i32imm:$amt2),
                                     "", [(callseq_end timm:$amt1, timm:$amt2)]> {
  let isCodeGenOnly = 1;
}

// A real 'ADD' instruction. Its pattern (set/add) is also now visible.
def ADD : SimpleRISCInst<(outs GPR:$rd), (ins GPR:$rs1, GPR:$rs2),
                         "add $rd, $rs1, $rs2",
                         [(set GPR:$rd, (add GPR:$rs1, GPR:$rs2))]>;
