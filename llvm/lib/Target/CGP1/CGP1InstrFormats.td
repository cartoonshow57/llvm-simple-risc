//===----------------------------------------------------------------------===//
// CGP1 GPU Instruction Formats
//
// All instructions are 32-bit with the following general format:
//
// 31 30 29 28 27 | 26 25 24 | 23 22 21 20 | 19 18 17 16 | 15 14 13 12 | 11 10 09 08 | 07 06 05 04 | 03 02 01 00
// |  opcode (5)  |  cond(3) |   k/k_ra(4) |   sel(4)    |    ra(4)    |    rb(4)    |    rc(4)    |  unused(4) |
//
// Fields:
//   opcode  : 5-bit instruction opcode (bits 31-27)
//   cond    : 3-bit condition code for conditional execution (bits 26-24)
//   k/k_ra  : 4-bit immediate or extended field (bits 23-20)
//   sel     : 4-bit selection/modifier field (bits 19-16)
//   ra      : 4-bit source/destination register A (bits 15-12)
//   rb      : 4-bit source register B (bits 11-8)
//   rc      : 4-bit destination/source register C (bits 7-4)
//   unused  : 4-bit unused/reserved (bits 3-0)
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Base Instruction Class
//===----------------------------------------------------------------------===//

class CGP1Inst<dag outs, dag ins, string asmString, list<dag> pattern = []> 
    : Instruction {
  let Namespace = "CGP1";
  let OutOperandList = outs;
  let InOperandList = ins;
  let AsmString = asmString;
  let Pattern = pattern;
  let Size = 4;  // All instructions are 32-bit (4 bytes)
  
  // Instruction fields
  bits<5> opcode = 0;
  bits<3> cond = 0;    // Default: always execute (AL)
  bits<4> k = 0;
  bits<4> sel = 0;
  bits<4> ra = 0;
  bits<4> rb = 0;
  bits<4> rc = 0;
}

//===----------------------------------------------------------------------===//
// Pseudo Instructions (not emitted to machine code)
//===----------------------------------------------------------------------===//

class PseudoCGP1Inst<dag outs, dag ins, string asmString, list<dag> pattern = []> 
    : CGP1Inst<outs, ins, asmString, pattern> {
  let isPseudo = 1;
  let isCodeGenOnly = 1;
}

//===----------------------------------------------------------------------===//
// Arithmetic Instruction Formats (3-operand: rc = ra op rb)
//===----------------------------------------------------------------------===//

// Format for 3-register operations: op rc, ra, rb
class CGP1InstArith<bits<5> op, bits<4> selVal, dag outs, dag ins, 
                    string asmString, list<dag> pattern = []>
    : CGP1Inst<outs, ins, asmString, pattern> {
  let opcode = op;
  let sel = selVal;
}

// Format for 2-register operations: op rc, ra (unary)
class CGP1InstUnary<bits<5> op, bits<4> selVal, dag outs, dag ins,
                    string asmString, list<dag> pattern = []>
    : CGP1Inst<outs, ins, asmString, pattern> {
  let opcode = op;
  let sel = selVal;
}

//===----------------------------------------------------------------------===//
// Memory Instruction Formats
//===----------------------------------------------------------------------===//

// Load format: load rc, [ra + offset]
class CGP1InstLoad<bits<5> op, dag outs, dag ins, string asmString, 
                   list<dag> pattern = []>
    : CGP1Inst<outs, ins, asmString, pattern> {
  let opcode = op;
  let mayLoad = 1;
  let canFoldAsLoad = 1;
}

// Store format: store [ra + offset], rb
class CGP1InstStore<bits<5> op, dag outs, dag ins, string asmString,
                    list<dag> pattern = []>
    : CGP1Inst<outs, ins, asmString, pattern> {
  let opcode = op;
  let mayStore = 1;
}

//===----------------------------------------------------------------------===//
// Control Flow Instruction Formats
//===----------------------------------------------------------------------===//

// Jump/Branch format
class CGP1InstBranch<bits<5> op, dag outs, dag ins, string asmString,
                     list<dag> pattern = []>
    : CGP1Inst<outs, ins, asmString, pattern> {
  let opcode = op;
  let isBranch = 1;
  let isTerminator = 1;
}

// Call format
class CGP1InstCall<bits<5> op, dag outs, dag ins, string asmString,
                   list<dag> pattern = []>
    : CGP1Inst<outs, ins, asmString, pattern> {
  let opcode = op;
  let isCall = 1;
  let Defs = [R14];  // LR is clobbered
}

// Return format
class CGP1InstReturn<bits<5> op, dag outs, dag ins, string asmString,
                     list<dag> pattern = []>
    : CGP1Inst<outs, ins, asmString, pattern> {
  let opcode = op;
  let isReturn = 1;
  let isTerminator = 1;
  let isBarrier = 1;
  let Uses = [R14];  // Uses LR
}

//===----------------------------------------------------------------------===//
// Condition Codes
//===----------------------------------------------------------------------===//
// 
// cond field (3 bits):
//   000 = AL (always)
//   001 = EQ (equal)
//   010 = NE (not equal)
//   011 = LT (less than)
//   100 = LE (less or equal)
//   101 = GT (greater than)
//   110 = GE (greater or equal)
//   111 = reserved
//===----------------------------------------------------------------------===//
