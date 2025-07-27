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

// This is a "leaf" pattern. It's a rule used inside other instruction
// patterns to check if an immediate value is a 16-bit signed number.
// This is generic and doesn't need to be changed.
def immSExt16 : PatLeaf<(imm), [{
  return isInt<16>(N->getSExtValue());
}]>;

// This is a "complex pattern" for matching memory addresses. It's more
// powerful than a simple operand definition. It tells the instruction selector
// to call a custom C++ function to handle the matching logic.
def addr : ComplexPattern<iPTR, 2, "SelectSimpleRISCAddr",
                         [frameindex, globaladdr], []>;

//===----------------------------------------------------------------------===//
// Pattern fragments for checking memory alignment
//===----------------------------------------------------------------------===//
// These 'PatFrag's (Pattern Fragments) define reusable conditions that can be
// used in our load and store instruction patterns. They check if a memory
// access is "naturally aligned" (e.g., a 4-byte load from an address that is
// a multiple of 4). This is very important for many RISC architectures.

// Predicate to check if a Load is aligned.
class AlignedLoad :
  PatFrag<(ops node:$ptr), (load node:$ptr), [{
    LoadSDNode *LD = cast<LoadSDNode>(N);
    return LD->getMemoryVT().getSizeInBits()/8 <= LD->getAlignment();
  }]>;

// Predicate to check if a Store is aligned.
class AlignedStore :
  PatFrag<(ops node:$val, node:$ptr), (store node:$val, node:$ptr), [{
    StoreSDNode *SD = cast<StoreSDNode>(N);
    return SD->getMemoryVT().getSizeInBits()/8 <= SD->getAlignment();
  }]>;

// Define specific instances of our alignment-checking fragments.
// We can now use 'load_a' and 'store_a' in our instruction patterns.
def load_a : AlignedLoad;
def store_a : AlignedStore;

// In SimpleRISCInstrInfo.td

// First, make sure you include the file with all your format and opcode definitions.
include "SimpleRISCInstFormats.td"

// Now, let's define the ADD instruction.
// We inherit from the 'InstR' class we created earlier.
def ADD : InstR<(outs GPR:$rd), (ins GPR:$rs1, GPR:$rs2),
                "add $rd, $rs1, $rs2",
                [(set GPR:$rd, (add GPR:$rs1, GPR:$rs2))]> {
  // Here, we use the opcode we defined in the formats file.
  // We are setting the 5-bit 'opcode' field from the InstR class
  // to the value of OPC_ADD.
  let opcode = OPC_ADD.Value;
}

// You would then do the same for all your other instructions.
// For example, for SUB:
def SUB : InstR<(outs GPR:$rd), (ins GPR:$rs1, GPR:$rs2),
                "sub $rd, $rs1, $rs2",
                [(set GPR:$rd, (sub GPR:$rs1, GPR:$rs2))]> {
  let opcode = OPC_SUB.Value;
}

//===----------------------------------------------------------------------===//
//  SimpleRISC Load/Store Instruction Classes
//===----------------------------------------------------------------------===//

// This is a template for all Load instructions.
// It inherits from InstMem, our format for memory operations.
class LoadM<PatFrag OpNode, RegisterClass RC, Operand MemOpnd>
  : InstMem<(outs RC:$rt), (ins MemOpnd:$addr),
            "ld $rt, $addr",
            [(set RC:$rt, (OpNode addr:$addr))]> {

  // This property tells the compiler that this instruction is a load,
  // which enables certain optimizations.
  let canFoldAsLoad = 1;
}

// This is a template for all Store instructions.
// It also inherits from InstMem.
class StoreM<PatFrag OpNode, RegisterClass RC, Operand MemOpnd>
  : InstMem<(outs), (ins RC:$rt, MemOpnd:$addr),
            "st $rt, $addr",
            [(OpNode RC:$rt, addr:$addr)]>;


//===----------------------------------------------------------------------===//
//  SimpleRISC 32-bit Specific Load/Store Classes
//===----------------------------------------------------------------------===//

// This class specializes the generic LoadM template for 32-bit loads.
// It automatically fills in the RegisterClass with GPR (our 32-bit
// general-purpose registers) and the memory operand with 'addr'.
class LoadM32<PatFrag OpNode> : LoadM<OpNode, GPR, addr>;

// This class specializes the generic StoreM template for 32-bit stores.
class StoreM32<PatFrag OpNode> : StoreM<OpNode, GPR, addr>;


//===----------------------------------------------------------------------===//
//  SimpleRISC Jump/Return Instruction Classes
//===----------------------------------------------------------------------===//

// First, we need a new binary format for Jump-Register instructions.
// It uses an opcode and a single source register (rs1).
class InstJR<dag outs, dag ins, string asmstr, list<dag> pattern>
  : SimpleRISCInst<outs, ins, asmstr, pattern> {
  bits<5> opcode;
  bits<4> rs1;

  // Assemble the fields into the final 32-bit instruction.
  // The other register fields are unused (set to 0).
  let Inst = { 0b0000000000000000, rs1, 0b0000, opcode, 0b100 };
}

// This is a template for all indirect jump instructions.
// It inherits from our new InstJR format.
class JumpReg<dag outs, dag ins, string asmstr, list<dag> pattern>
  : InstJR<outs, ins, asmstr, pattern> {
  // These properties are crucial for the compiler to understand
  // that this is a control-flow instruction that ends a basic block.
  let isBranch = 1;
  let isTerminator = 1;
  let isBarrier = 1;
  let isIndirectBranch = 1; // Signifies a jump to a register's value.
}

//===----------------------------------------------------------------------===//
//  Return Instruction Definition
//===----------------------------------------------------------------------===//

// Now, we define the actual RET instruction using our JumpReg template.
// The pattern [(ret GPR:$rs)] tells the instruction selector how to match
// the high-level 'ret' operation to this machine instruction.
def RET : JumpReg<(outs), (ins GPR:$rs),
                  "ret $rs",
                  [(ret GPR:$rs)]> {
  // Set the specific opcode for this instruction.
  let opcode = 0b10100; // This is OPC_RET from your other file
}

// NOP
def NOP : InstR<(outs), (ins), "nop", []> {
  let opcode = OPC_NOP.Value;
  // A NOP can be modeled as an ADD that writes to a non-existent register.
  // We can set all register fields to 0.
  let rd = 0;
  let rs1 = 0;
  let rs2 = 0;
}

