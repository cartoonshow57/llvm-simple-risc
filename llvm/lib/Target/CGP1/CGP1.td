//@s create-cgp1-td removed
// // This is the top tablegen file that pulls in other code
// // specific to the CGP1 target.

// include "llvm/Target/Target.td"

// include "CGP1RegisterInfo.td"

// def : ProcessorModel<"generic", NoSchedModel, []>;

// def CGP1 : Target {
// }
//- create-cgp1-td


include "llvm/Target/Target.td"

include "CGP1RegisterInfo.td"
//@s cgp1-instr-info-td-include
include "CGP1InstrInfo.td"
//- cgp1-instr-info-td-include

//@s cgp1-calling-conv-td-include
include "CGP1CallingConv.td"
//- cgp1-calling-conv-td-include

def : ProcessorModel<"generic", NoSchedModel, []>;

def CGP1InstrInfo : InstrInfo {}

def CGP1 : Target {
  let InstructionSet = CGP1InstrInfo;
}