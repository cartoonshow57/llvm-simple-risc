//===-- SimpleRISCInstrInfo.td - Instructions -------------*- tablegen -*-===//
//
// This file describes the SimpleRISC instruction set.
//
//===----------------------------------------------------------------------===//

def SDT_SimpleRISCRet : SDTypeProfile<0, 1, [SDTCisInt<0>]>;

def SimpleRISCRet : SDNode<"SimpleRISCISD::Ret", SDT_SimpleRISCRet,
                     [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

//===----------------------------------------------------------------------===//
// Instruction format superclass
//===----------------------------------------------------------------------===//

include "SimpleRISCInstFormats.td"

//===----------------------------------------------------------------------===//
// SimpleRISC Operand, Complex Patterns and Transformations Definitions.
//===----------------------------------------------------------------------===//

// A 16-bit signed immediate operand.
// This tells the disassembler to call a C++ function named
// 'DecodeSimm16' when it encounters this type of operand.
def simm16 : Operand<i32> {
  let DecoderMethod = "DecodeSimm16";
}

// A memory address operand. This is the most important part.
// It defines the [base_register + immediate_offset] addressing mode.
def mem : Operand<iPTR> {
  // Tell the instruction printer to call our custom C++ function
  // to print memory operands correctly (e.g., "100(r2)").
  let PrintMethod = "printSimpleRISCMemOperand";

  // This defines the structure of the 'mem' operand for the compiler.
  // It's made of two parts: a general-purpose register and a 16-bit immediate.
  let MIOperandInfo = (ops GPR, simm16);

  // Tell the assembler to call our custom C++ function to encode
  // this memory operand into its final binary format.
  let EncoderMethod = "getSimpleRISCMemOperand";
}