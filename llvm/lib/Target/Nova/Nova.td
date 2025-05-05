//@s create-nova-td removed
// // This is the top tablegen file that pulls in other code
// // specific to the Nova target.

// include "llvm/Target/Target.td"

// include "NovaRegisterInfo.td"

// def : ProcessorModel<"generic", NoSchedModel, []>;

// def Nova : Target {
// }
//- create-nova-td

include "llvm/Target/Target.td"

include "NovaRegisterInfo.td"
//@s nova-instr-info-td-include
include "NovaInstrInfo.td"
//- nova-instr-info-td-include

def : ProcessorModel<"generic", NoSchedModel, []>;

def NovaInstrInfo : InstrInfo {}

def Nova : Target {
  let InstructionSet = NovaInstrInfo;
}