//===-- SimpleRISC.td - Target Description for SimpleRISC ---*- tablegen -*-===//
//
// This is the top-level file for the SimpleRISC target description.
//
//===----------------------------------------------------------------------===//

// This single include is sufficient. It brings in all necessary base
// definitions for a target.
include "llvm/Target/Target.td"

//===----------------------------------------------------------------------===//
// Register File Description
//===----------------------------------------------------------------------===//

class SimpleRISCReg<string n> : Register<n>;

// Define the 8 General Purpose Registers (GPRs)
def R0 : SimpleRISCReg<"r0">;
def R1 : SimpleRISCReg<"r1">;
def R2 : SimpleRISCReg<"r2">;
def R3 : SimpleRISCReg<"r3">;
def R4 : SimpleRISCReg<"r4">;
def R5 : SimpleRISCReg<"r5">;
def R6 : SimpleRISCReg<"r6">;
def R7 : SimpleRISCReg<"r7">;

// Define the RegisterClass that groups all our GPRs.
def GPR : RegisterClass<"SimpleRISC", [i32], 32, (add
  R0, R1, R2, R3, R4, R5, R6, R7
)>;

//===----------------------------------------------------------------------===//
// Instruction Format Superclass
//===----------------------------------------------------------------------===//

class SimpleRISCInst<dag outs, dag ins, string asmstr, list<dag> pattern>
  : Instruction {
  let OutOperandList = outs;
  let InOperandList  = ins;
  let AsmString      = asmstr;
  let Pattern        = pattern;
}

//===----------------------------------------------------------------------===//
// Instruction Definitions
//===----------------------------------------------------------------------===//

// These pseudo-instructions are required by LLVM for stack management.
// They are marked with 'isCodeGenOnly = 1' to signify that they are for
// the compiler's internal use and do not get encoded into machine code.
def ADJCALLSTACKDOWN : SimpleRISCInst<(outs), (ins i32imm:$amt1, i32imm:$amt2),
                                     "", []> {
  let isCodeGenOnly = 1;
}
def ADJCALLSTACKUP   : SimpleRISCInst<(outs), (ins i32imm:$amt1, i32imm:$amt2),
                                     "", []> {
  let isCodeGenOnly = 1;
}

// A real 'ADD' instruction. The 'Pattern' field tells the instruction selector
// how to match this instruction to the high-level LLVM IR.
def ADD : SimpleRISCInst<(outs GPR:$rd), (ins GPR:$rs1, GPR:$rs2),
                         "add $rd, $rs1, $rs2",
                         [(set GPR:$rd, (add GPR:$rs1, GPR:$rs2))]>;

//===----------------------------------------------------------------------===//
// Processor and Subtarget Information
//===----------------------------------------------------------------------===//

def SimpleRISCProcessor : ProcessorModel<"simplerisc", NoSchedModel, []>;

//===----------------------------------------------------------------------===//
// Target Declaration
//===----------------------------------------------------------------------===//

def SimpleRISCInstrInfo : InstrInfo;

def SimpleRISCAsmWriter : AsmWriter {
  int PassSubtarget = 0;
}

def SimpleRISC : Target {
  let InstructionSet = SimpleRISCInstrInfo;
  let AssemblyWriters = [SimpleRISCAsmWriter];
}
