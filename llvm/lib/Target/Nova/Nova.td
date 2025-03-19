//@s create-nova-td end
// This is the top tablegen file that pulls in other code
// specific to the Nova target.

include "llvm/Target/Target.td"

include "NovaRegisterInfo.td"

def : ProcessorModel<"generic", NoSchedModel, []>;

def Nova : Target {
}
//- create-nova-td