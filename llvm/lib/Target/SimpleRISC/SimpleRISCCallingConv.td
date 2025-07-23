//===-- SimpleRISCCallingConv.td - Calling Conventions ----*- tablegen -*-===//
//
// This file describes the calling conventions for the SimpleRISC architecture.
//
//===----------------------------------------------------------------------===//

// Includes the definitions for things like CCIfType, CCAssignToReg, etc.
include "llvm/Target/TargetCallingConv.td"

// This defines our standard calling convention.
def CC_SimpleRISC : CallingConv<[
  // Rule: If an argument is a 32-bit integer (i32), assign it to the
  // first available register from the sequence [R0, R1].
  CCIfType<[i32], CCAssignToReg<[R0, R1]>>
]>;
