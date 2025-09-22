//===- CGP1InstrInfo.td - CGP1 Instruction defs --------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file describes the CGP1 instruction set.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Operand and Complex Pattern Definitions
//===----------------------------------------------------------------------===//

// Defines an operand that is a 32-bit integer register from the GPR class.
def gpr : Operand<i32> {
  let PrintMethod = "printRegName";
  let MIOperandInfo = (ops GPR);
}

//===----------------------------------------------------------------------===//
// Instruction Format Superclasses
//===----------------------------------------------------------------------===//

// This is a base class for instructions. We assume a fixed 32-bit (4-byte)
// instruction size for simplicity.
// It defines the basic structure: outputs, inputs, assembly string, and the
// pattern to match in the LLVM IR.
class CGP1Inst<dag outs, dag ins, string asmstr, list<dag> pattern>
    : Instruction {
  field bits<32> Inst; // Placeholder for the binary encoding
  bit isCommutative = 0;
  let Namespace = "CGP1";
  let Size = 4; // Instructions are 4 bytes wide

  let OutOperandList = outs;
  let InOperandList = ins;
  let AsmString = asmstr;
  let Pattern = pattern;
}

// A specific format for R-type (Register-Register) instructions like ADD.
// It takes three register operands.
class R_TYPE<string opcodestr, SDPatternOperator op>
    : CGP1Inst<(outs GPR:$rd), (ins GPR:$rs1, GPR:$rs2),
               opcodestr # "\t$rd, $rs1, $rs2",
               [(set GPR:$rd, (op GPR:$rs1, GPR:$rs2))]>;

//===----------------------------------------------------------------------===//
// Instruction Definitions
//===----------------------------------------------------------------------===//

// The ADD instruction definition.
// It inherits from the R_TYPE class we defined above.
def ADD : R_TYPE<"add", add> {
  // Add properties specific to this instruction.
  // 'isCommutative' is a hint for the optimizer.
  let isCommutative = 1;
}

// The SUB instruction definition.
// It also uses the R_TYPE class. Subtraction is not commutative,
// so we don't need to set any special properties.
def SUB : R_TYPE<"sub", sub>;

//===----------------------------------------------------------------------===//
// Processor and Calling Convention
//===----------------------------------------------------------------------===//

// Define the calling convention for CGP1.
// This describes how function arguments and return values are passed.
// It's based on the register roles you defined in CGP1RegisterInfo.td.
def CC_CGP1 : CallingConv<[
  // Integer arguments are passed in these registers, in order.
  CCIfType<[i32], CCAssignToReg<[R6, R7, R18, R19]>>,

  // Any further arguments are passed on the stack.
  CCAssignToStack<4, 4>
]>;

// Define how values are returned from functions.
def RetCC_CGP1 : CallingConv<[
  // 32-bit integer return values are placed in the RV register (R8).
  CCIfType<[i32], CCAssignToReg<[RV]>>
]>;

// Add this to CGP1InstrInfo.td

// Define the main instruction info record for the CGP1 target.
// This record acts as the container for all instruction information.
def CGP1InstrInfo : InstrInfo {
  // We can leave this empty for now. Its existence is what matters.
}