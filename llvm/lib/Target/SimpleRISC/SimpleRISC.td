//===-- SimpleRISC.td - Target Description for SimpleRISC ---*- tablegen -*-===//
//
// This is the top-level file for the SimpleRISC target description. It's
// responsible for including the other definition files in the correct order.
//
//===----------------------------------------------------------------------===//

// This include must come first. It defines base classes like 'Target',
// 'Instruction', and 'ValueType', and it also includes TargetSelectionDAG.td.
include "llvm/Target/Target.td"

// Include our new, separated definition files.
// include "SimpleRISCRegisterInfo.td"
// include "SimpleRISCCallingConv.td"
include "SimpleRISCInstrInfo.td"

//===----------------------------------------------------------------------===//
// Processor and Subtarget Information
//===----------------------------------------------------------------------===//

def SimpleRISCProcessor : ProcessorModel<"simplerisc", NoSchedModel, []>;

//===----------------------------------------------------------------------===//
// Target Declaration
//===----------------------------------------------------------------------===//

def SimpleRISCInstrInfo : InstrInfo;

// def SimpleRISCAsmWriter : AsmWriter {
//   int PassSubtarget = 0;
// }

def SimpleRISC : Target {
  let InstructionSet = SimpleRISCInstrInfo;
  // let AssemblyWriters = [SimpleRISCAsmWriter];
}