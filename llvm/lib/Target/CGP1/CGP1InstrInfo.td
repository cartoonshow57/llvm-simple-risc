//===----------------------------------------------------------------------===//
// CGP1 GPU Instruction Definitions
// 
// This file defines the instructions for the CGP1 GPU ISA.
// See CGP1_ISA.md for the complete ISA specification.
//===----------------------------------------------------------------------===//

include "CGP1InstrFormats.td"

//===----------------------------------------------------------------------===//
// SDNode Definitions
//===----------------------------------------------------------------------===//

// Return node
def CGP1RetSDN : SDNode<"CGP1ISD::Ret", SDTNone,
                        [SDNPHasChain, SDNPVariadic, SDNPOptInGlue]>;

// Function call node
def SDT_CGP1Call : SDTypeProfile<0, 1, [SDTCisVT<0, iPTR>]>;
def CGP1call : SDNode<"CGP1ISD::Call", SDT_CGP1Call,
                      [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue, SDNPVariadic]>;

// Global address wrapper
def CGP1wrapper : SDNode<"CGP1ISD::Wrapper", SDTIntUnaryOp>;

// Hi/Lo for address materialization
def CGP1hi : SDNode<"CGP1ISD::Hi", SDTIntUnaryOp>;
def CGP1lo : SDNode<"CGP1ISD::Lo", SDTIntUnaryOp>;

// GPU barrier for thread synchronization
def CGP1barrier : SDNode<"CGP1ISD::Barrier", SDTNone, [SDNPHasChain, SDNPSideEffect]>;

// Kernel termination node (STOP_CORE) - terminates GPU kernel execution
def CGP1endkernel : SDNode<"CGP1ISD::EndKernel", SDTNone, [SDNPHasChain]>;

// SIMT EXEC mask stack operations
def SDT_CGP1ExecPush : SDTypeProfile<0, 1, [SDTCisVT<0, i32>]>;
def SDT_CGP1ExecPop : SDTypeProfile<1, 0, [SDTCisVT<0, i32>]>;
def SDT_CGP1ExecReconverge : SDTypeProfile<0, 0, []>;
def CGP1execpush : SDNode<"CGP1ISD::ExecPush", SDT_CGP1ExecPush,
                          [SDNPHasChain, SDNPMayStore, SDNPSideEffect]>;
def CGP1execpop : SDNode<"CGP1ISD::ExecPop", SDT_CGP1ExecPop,
                         [SDNPHasChain, SDNPMayLoad, SDNPSideEffect]>;
def CGP1execreconverge : SDNode<"CGP1ISD::ExecReconverge", SDT_CGP1ExecReconverge,
                                [SDNPHasChain, SDNPMayLoad, SDNPSideEffect]>;

// Explicitly defined for reconvergence (pops EXEC mask from stack to R15)
let Defs = [R15, R13], Uses = [R13] in
def EXEC_RECONVERGE : CGP1Inst<(outs), (ins), "pop\tr15", [(CGP1execreconverge)]> {
  let opcode = 0b10011;
  // TODO: Set register field to R15 (1111)
}

// Graphics operation SDNodes
def CGP1clearscreen : SDNode<"CGP1ISD::ClearScreen", SDTNone, [SDNPHasChain, SDNPSideEffect]>;
def CGP1drawline : SDNode<"CGP1ISD::DrawLine", SDTNone, [SDNPHasChain, SDNPSideEffect, SDNPOptInGlue]>;
def CGP1drawtriangle : SDNode<"CGP1ISD::DrawTriangle", SDTNone, [SDNPHasChain, SDNPSideEffect, SDNPOptInGlue]>;

// Conditional branch nodes
def SDT_CGP1BrCC : SDTypeProfile<0, 2, [SDTCisVT<0, i32>, SDTCisVT<1, OtherVT>]>;
def CGP1brz  : SDNode<"CGP1ISD::BranchZero", SDT_CGP1BrCC, [SDNPHasChain]>;
def CGP1brnz : SDNode<"CGP1ISD::BranchNotZero", SDT_CGP1BrCC, [SDNPHasChain]>;
def CGP1brltz : SDNode<"CGP1ISD::BranchLTZ", SDT_CGP1BrCC, [SDNPHasChain]>;
def CGP1brgez : SDNode<"CGP1ISD::BranchGEZ", SDT_CGP1BrCC, [SDNPHasChain]>;

// SIMT branch node: handles both taken and fall-through paths with EXEC mask management
// Operands: Chain, ConditionMask, TakenExec, FallThroughExec, TakenDest, FallThroughDest
def SDT_CGP1SIMTBranch : SDTypeProfile<0, 6, [SDTCisVT<0, OtherVT>, SDTCisVT<1, i32>, 
                                            SDTCisVT<2, i32>, SDTCisVT<3, i32>,
                                            SDTCisVT<4, OtherVT>, SDTCisVT<5, OtherVT>]>;
def CGP1simtbr : SDNode<"CGP1ISD::SIMTBranch", SDT_CGP1SIMTBranch, [SDNPHasChain, SDNPSideEffect]>;

// Float immediate node (loads float as its integer bit representation)
def SDT_CGP1FPImm : SDTypeProfile<1, 1, [SDTCisVT<0, f32>, SDTCisVT<1, i32>]>;
def CGP1fpimm : SDNode<"CGP1ISD::FPImm", SDT_CGP1FPImm>;

// Atomic nodes
def SDT_CGP1AtomicBinary : SDTypeProfile<1, 2, [SDTCisVT<0, i32>, SDTCisPtrTy<1>, SDTCisVT<2, i32>]>;
def SDT_CGP1AtomicTernary : SDTypeProfile<1, 3, [SDTCisVT<0, i32>, SDTCisPtrTy<1>, SDTCisVT<2, i32>, SDTCisVT<3, i32>]>;

def CGP1atomic_add : SDNode<"CGP1ISD::AtomicAdd", SDT_CGP1AtomicBinary, [SDNPHasChain, SDNPMayStore, SDNPMayLoad, SDNPSideEffect]>;
def CGP1atomic_cas : SDNode<"CGP1ISD::AtomicCas", SDT_CGP1AtomicTernary, [SDNPHasChain, SDNPMayStore, SDNPMayLoad, SDNPSideEffect]>;
def CGP1atomic_max : SDNode<"CGP1ISD::AtomicMax", SDT_CGP1AtomicBinary, [SDNPHasChain, SDNPMayStore, SDNPMayLoad, SDNPSideEffect]>;
def CGP1atomic_min : SDNode<"CGP1ISD::AtomicMin", SDT_CGP1AtomicBinary, [SDNPHasChain, SDNPMayStore, SDNPMayLoad, SDNPSideEffect]>;
def CGP1atomic_exch : SDNode<"CGP1ISD::AtomicExch", SDT_CGP1AtomicBinary, [SDNPHasChain, SDNPMayStore, SDNPMayLoad, SDNPSideEffect]>;

// Warp nodes
def SDT_CGP1WarpUnary : SDTypeProfile<1, 1, [SDTCisVT<0, i32>, SDTCisVT<1, i32>]>;
def SDT_CGP1WarpBinary : SDTypeProfile<1, 2, [SDTCisVT<0, i32>, SDTCisVT<1, i32>, SDTCisVT<2, i32>]>;
def SDT_CGP1WarpNone : SDTypeProfile<1, 0, [SDTCisVT<0, i32>]>;

def CGP1warp_size : SDNode<"CGP1ISD::WarpSize", SDT_CGP1WarpNone>;
def CGP1lane_id : SDNode<"CGP1ISD::LaneId", SDT_CGP1WarpNone>;
def CGP1warp_all : SDNode<"CGP1ISD::WarpAll", SDT_CGP1WarpUnary>;
def CGP1warp_any : SDNode<"CGP1ISD::WarpAny", SDT_CGP1WarpUnary>;
def CGP1warp_ballot : SDNode<"CGP1ISD::WarpBallot", SDT_CGP1WarpUnary>;
def CGP1warp_shuffle : SDNode<"CGP1ISD::WarpShuffle", SDT_CGP1WarpBinary>;

//===----------------------------------------------------------------------===//
// Operand Definitions
//===----------------------------------------------------------------------===//

// Signed 16-bit immediate
def simm16 : Operand<i32>;

// Unsigned 16-bit immediate
def uimm16 : Operand<i32>;

// 4-bit immediate (for k field)
def uimm4 : Operand<i32>;

// Branch target
def brtarget : Operand<OtherVT>;

// Call target
def calltarget : Operand<i32>;

// Memory operand: base register + offset
def mem : Operand<i32> {
  let PrintMethod = "printMemOperand";
  let MIOperandInfo = (ops GPR:$base, simm16:$offset);
}

// Complex pattern for address selection
def addr : ComplexPattern<i32, 2, "SelectAddr", []>;

//===----------------------------------------------------------------------===//
// NOP Instruction (opcode 00000)
//===----------------------------------------------------------------------===//

let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in
def NOP : CGP1Inst<(outs), (ins), "nop", []> {
  let opcode = 0b00000;
}

//===----------------------------------------------------------------------===//
// Load Immediate Instructions (opcode 00001)
//===----------------------------------------------------------------------===//

// ploadf_l: Partial load float low 16 bits
def PLOADF_L : CGP1Inst<(outs GPR:$rc), (ins i32imm:$imm),
                        "ploadf_l\t$rc, $imm", []> {
  let opcode = 0b00001;
}

// loadf_h: Load float high 16 bits
def LOADF_H : CGP1Inst<(outs GPR:$rc), (ins GPR:$ra, i32imm:$imm),
                       "loadf_h\t$rc, $ra, $imm", []> {
  let opcode = 0b00001;
}

// ploadu_l: Partial load unsigned low 16 bits
def PLOADU_L : CGP1Inst<(outs GPR:$rc), (ins i32imm:$imm),
                        "ploadu_l\t$rc, $imm", []> {
  let opcode = 0b00001;
}

// loadu_h: Load unsigned high 16 bits
def LOADU_H : CGP1Inst<(outs GPR:$rc), (ins GPR:$ra, i32imm:$imm),
                       "loadu_h\t$rc, $ra, $imm", []> {
  let opcode = 0b00001;
}

// ploadaddr_l: Partial load address low 16 bits
def PLOADADDR_L : CGP1Inst<(outs GPR:$rc), (ins i32imm:$imm),
                           "ploadaddr_l\t$rc, $imm", []> {
  let opcode = 0b00001;
}

// loadaddr_h: Load address high 16 bits
def LOADADDR_H : CGP1Inst<(outs GPR:$rc), (ins GPR:$ra, i32imm:$imm),
                          "loadaddr_h\t$rc, $ra, $imm", []> {
  let opcode = 0b00001;
}

//===----------------------------------------------------------------------===//
// Data Movement Instructions (opcode 00010)
//===----------------------------------------------------------------------===//

// copy: Register copy (rc = ra) - opcode 00010
let hasSideEffects = 0 in
def COPY_REG : CGP1InstUnary<0b00010, 0b0000, 
                              (outs GPR:$rc), (ins GPR:$ra),
                              "copy\t$rc, $ra", []>;

// Load immediate (pseudo-instruction for constant materialization)
// Uses ploadu_l instruction (opcode 00010) to load 16-bit unsigned immediate
def LOADI : CGP1Inst<(outs GPR:$rc), (ins i32imm:$imm),
                     "loadi\t$rc, $imm",
                     [(set GPR:$rc, imm:$imm)]> {
  let opcode = 0b00010;
  let isPseudo = 1;
}

// Load float immediate (pseudo-instruction for float constant materialization)
// Loads the integer bit representation of a float into a register
def LOADFI : CGP1Inst<(outs GPR:$rc), (ins i32imm:$imm),
                      "loadi\t$rc, $imm",
                      [(set GPR:$rc, (CGP1fpimm timm:$imm))]> {
  let opcode = 0b00010;
  let isPseudo = 1;
}

// fabs: Floating-point absolute value (rc = |ra|) - opcode 00010
def FABS : CGP1InstUnary<0b00010, 0b0001,
                         (outs GPR:$rc), (ins GPR:$ra),
                         "fabs\t$rc, $ra",
                         [(set GPR:$rc, (fabs GPR:$ra))]>;

// fneg: Floating-point negate (rc = -ra) - opcode 00010
def FNEG : CGP1InstUnary<0b00010, 0b0010,
                         (outs GPR:$rc), (ins GPR:$ra),
                         "fneg\t$rc, $ra",
                         [(set GPR:$rc, (fneg GPR:$ra))]>;

// fnabs: Floating-point negate absolute (rc = -|ra|) - opcode 00010
def FNABS : CGP1InstUnary<0b00010, 0b0011,
                          (outs GPR:$rc), (ins GPR:$ra),
                          "fnabs\t$rc, $ra", []>;

//===----------------------------------------------------------------------===//
// Floating-Point Arithmetic Instructions (opcodes 00100-00110)
//===----------------------------------------------------------------------===//

// fadd: Floating-point add (rc = ra + rb) - opcode 00100, sel=0
def FADD : CGP1InstArith<0b00100, 0b0000,
                         (outs GPR:$rc), (ins GPR:$ra, GPR:$rb),
                         "fadd\t$rc, $ra, $rb",
                         [(set GPR:$rc, (fadd GPR:$ra, GPR:$rb))]>;

// fsub: Floating-point subtract (rc = ra - rb) - opcode 00100, sel=1
def FSUB : CGP1InstArith<0b00100, 0b0001,
                         (outs GPR:$rc), (ins GPR:$ra, GPR:$rb),
                         "fsub\t$rc, $ra, $rb",
                         [(set GPR:$rc, (fsub GPR:$ra, GPR:$rb))]>;

// fmul: Floating-point multiply (rc = ra * rb) - opcode 00101
def FMUL : CGP1InstArith<0b00101, 0b0000,
                         (outs GPR:$rc), (ins GPR:$ra, GPR:$rb),
                         "fmul\t$rc, $ra, $rb",
                         [(set GPR:$rc, (fmul GPR:$ra, GPR:$rb))]>;

// fdiv: Floating-point divide (rc = ra / rb) - opcode 00110
def FDIV : CGP1InstArith<0b00110, 0b0000,
                         (outs GPR:$rc), (ins GPR:$ra, GPR:$rb),
                         "fdiv\t$rc, $ra, $rb",
                         [(set GPR:$rc, (fdiv GPR:$ra, GPR:$rb))]>;

//===----------------------------------------------------------------------===//
// FP Multiply Power of 2 (opcode 00011)
//===----------------------------------------------------------------------===//

// fmulp2: Floating-point multiply by power of 2
def FMULP2 : CGP1InstArith<0b00011, 0b0000,
                           (outs GPR:$rc), (ins GPR:$ra, GPR:$rb),
                           "fmulp2\t$rc, $ra, $rb", []>;

//===----------------------------------------------------------------------===//
// FP Utility Instructions (opcode 00111)
//===----------------------------------------------------------------------===//

// fmin: Floating-point minimum
def FMIN : CGP1InstArith<0b00111, 0b0000,
                         (outs GPR:$rc), (ins GPR:$ra, GPR:$rb),
                         "fmin\t$rc, $ra, $rb",
                         [(set GPR:$rc, (fminnum GPR:$ra, GPR:$rb))]>;

// fmax: Floating-point maximum
def FMAX : CGP1InstArith<0b00111, 0b0001,
                         (outs GPR:$rc), (ins GPR:$ra, GPR:$rb),
                         "fmax\t$rc, $ra, $rb",
                         [(set GPR:$rc, (fmaxnum GPR:$ra, GPR:$rb))]>;

// trunc: Truncate to integer - opcode 01000
def FTRUNC : CGP1InstUnary<0b01000, 0b0000,
                           (outs GPR:$rc), (ins GPR:$ra),
                           "trunc\t$rc, $ra",
                           [(set GPR:$rc, (ftrunc GPR:$ra))]>;

// round: Round to nearest integer - opcode 01000
def FROUND : CGP1InstUnary<0b01000, 0b0001,
                           (outs GPR:$rc), (ins GPR:$ra),
                           "round\t$rc, $ra",
                           [(set GPR:$rc, (fround GPR:$ra))]>;

// ceil: Ceiling - opcode 01000
def FCEIL : CGP1InstUnary<0b01000, 0b0010,
                          (outs GPR:$rc), (ins GPR:$ra),
                          "ceil\t$rc, $ra",
                          [(set GPR:$rc, (fceil GPR:$ra))]>;

// floor: Floor - opcode 01000
def FFLOOR : CGP1InstUnary<0b01000, 0b0011,
                           (outs GPR:$rc), (ins GPR:$ra),
                           "floor\t$rc, $ra",
                           [(set GPR:$rc, (ffloor GPR:$ra))]>;

//===----------------------------------------------------------------------===//
// Integer/FP Conversion Instructions (opcode 01000)
//===----------------------------------------------------------------------===//

// u32tof: Unsigned 32-bit integer to float - opcode 01110
def U32TOF : CGP1InstUnary<0b01110, 0b0000,
                           (outs GPR:$rc), (ins GPR:$ra),
                           "u32tof\t$rc, $ra",
                           [(set GPR:$rc, (uint_to_fp GPR:$ra))]>;

// ftou32: Float to unsigned 32-bit integer - opcode 01011
def FTOU32 : CGP1InstUnary<0b01011, 0b0000,
                           (outs GPR:$rc), (ins GPR:$ra),
                           "ftou32\t$rc, $ra",
                           [(set GPR:$rc, (fp_to_uint GPR:$ra))]>;

//===----------------------------------------------------------------------===//
// Float to 8-bit Conversion Instructions (opcode 01001)
//===----------------------------------------------------------------------===//

def FTOU8_LL : CGP1InstUnary<0b01001, 0b0000,
                             (outs GPR:$rc), (ins GPR:$ra),
                             "ftou8_ll\t$rc, $ra", []>;

def FTOU8_LH : CGP1InstUnary<0b01001, 0b0001,
                             (outs GPR:$rc), (ins GPR:$ra),
                             "ftou8_lh\t$rc, $ra", []>;

def FTOU8_HL : CGP1InstUnary<0b01001, 0b0010,
                             (outs GPR:$rc), (ins GPR:$ra),
                             "ftou8_hl\t$rc, $ra", []>;

def FTOU8_HH : CGP1InstUnary<0b01001, 0b0011,
                             (outs GPR:$rc), (ins GPR:$ra),
                             "ftou8_hh\t$rc, $ra", []>;

//===----------------------------------------------------------------------===//
// Float to 16-bit Conversion Instructions (opcode 01010)
//===----------------------------------------------------------------------===//

def FTOU16_L : CGP1InstUnary<0b01010, 0b0000,
                             (outs GPR:$rc), (ins GPR:$ra),
                             "ftou16_l\t$rc, $ra", []>;

def FTOU16_H : CGP1InstUnary<0b01010, 0b0001,
                             (outs GPR:$rc), (ins GPR:$ra),
                             "ftou16_h\t$rc, $ra", []>;

//===----------------------------------------------------------------------===//
// 8-bit to Float Conversion Instructions (opcode 01100)
//===----------------------------------------------------------------------===//

def U8TOF_LL : CGP1InstUnary<0b01100, 0b0000,
                             (outs GPR:$rc), (ins GPR:$ra),
                             "u8tof_ll\t$rc, $ra", []>;

def U8TOF_LH : CGP1InstUnary<0b01100, 0b0001,
                             (outs GPR:$rc), (ins GPR:$ra),
                             "u8tof_lh\t$rc, $ra", []>;

def U8TOF_HL : CGP1InstUnary<0b01100, 0b0010,
                             (outs GPR:$rc), (ins GPR:$ra),
                             "u8tof_hl\t$rc, $ra", []>;

def U8TOF_HH : CGP1InstUnary<0b01100, 0b0011,
                             (outs GPR:$rc), (ins GPR:$ra),
                             "u8tof_hh\t$rc, $ra", []>;

//===----------------------------------------------------------------------===//
// 16-bit to Float Conversion Instructions (opcode 01101)
//===----------------------------------------------------------------------===//

def U16TOF_L : CGP1InstUnary<0b01101, 0b0000,
                             (outs GPR:$rc), (ins GPR:$ra),
                             "u16tof_l\t$rc, $ra", []>;

def U16TOF_H : CGP1InstUnary<0b01101, 0b0001,
                             (outs GPR:$rc), (ins GPR:$ra),
                             "u16tof_h\t$rc, $ra", []>;

//===----------------------------------------------------------------------===//
// FP Comparison Instructions (opcode 01111)
//===----------------------------------------------------------------------===//

// fcomp_l: Compare less than - opcode 01111
def FCOMP_L : CGP1InstArith<0b01111, 0b0000,
                            (outs GPR:$rc), (ins GPR:$ra, GPR:$rb),
                            "fcomp_l\t$rc, $ra, $rb", []>;

// fcomp_le: Compare less than or equal - opcode 01111
def FCOMP_LE : CGP1InstArith<0b01111, 0b0001,
                             (outs GPR:$rc), (ins GPR:$ra, GPR:$rb),
                             "fcomp_le\t$rc, $ra, $rb", []>;

// fcomp_e: Compare equal - opcode 01111
def FCOMP_E : CGP1InstArith<0b01111, 0b0010,
                            (outs GPR:$rc), (ins GPR:$ra, GPR:$rb),
                            "fcomp_e\t$rc, $ra, $rb", []>;

// fcomp_ge: Compare greater than or equal - opcode 01111
def FCOMP_GE : CGP1InstArith<0b01111, 0b0011,
                             (outs GPR:$rc), (ins GPR:$ra, GPR:$rb),
                             "fcomp_ge\t$rc, $ra, $rb", []>;

// fcomp_g: Compare greater than - opcode 01111
def FCOMP_G : CGP1InstArith<0b01111, 0b0100,
                            (outs GPR:$rc), (ins GPR:$ra, GPR:$rb),
                            "fcomp_g\t$rc, $ra, $rb", []>;

// fcomp_ne: Compare not equal - opcode 01111
def FCOMP_NE : CGP1InstArith<0b01111, 0b0101,
                             (outs GPR:$rc), (ins GPR:$ra, GPR:$rb),
                             "fcomp_ne\t$rc, $ra, $rb", []>;

// fcomp_u: Compare unordered - opcode 01111
def FCOMP_U : CGP1InstArith<0b01111, 0b0110,
                            (outs GPR:$rc), (ins GPR:$ra, GPR:$rb),
                            "fcomp_u\t$rc, $ra, $rb", []>;

//===----------------------------------------------------------------------===//
// Integer Arithmetic Instructions (opcode 10000)
//===----------------------------------------------------------------------===//

// add: Integer add (rc = ra + rb) - opcode 10000
def ADD : CGP1InstArith<0b10000, 0b0000,
                        (outs GPR:$rc), (ins GPR:$ra, GPR:$rb),
                        "add\t$rc, $ra, $rb",
                        [(set GPR:$rc, (add GPR:$ra, GPR:$rb))]>;

// sub: Integer subtract (rc = ra - rb) - opcode 10000, sel=0001
def SUB : CGP1InstArith<0b10000, 0b0001,
                        (outs GPR:$rc), (ins GPR:$ra, GPR:$rb),
                        "sub\t$rc, $ra, $rb",
                        [(set GPR:$rc, (sub GPR:$ra, GPR:$rb))]>;

// mul: Integer multiply (rc = ra * rb) - opcode 10000, sel=0010
def MUL : CGP1InstArith<0b10000, 0b0010,
                        (outs GPR:$rc), (ins GPR:$ra, GPR:$rb),
                        "mul\t$rc, $ra, $rb",
                        [(set GPR:$rc, (mul GPR:$ra, GPR:$rb))]>;

// div: Integer divide (rc = ra / rb) - opcode 10000, sel=0011
def DIV : CGP1InstArith<0b10000, 0b0011,
                        (outs GPR:$rc), (ins GPR:$ra, GPR:$rb),
                        "div\t$rc, $ra, $rb",
                        [(set GPR:$rc, (sdiv GPR:$ra, GPR:$rb))]>;

// mod: Integer modulo (rc = ra % rb) - opcode 10000, sel=0100
def MOD : CGP1InstArith<0b10000, 0b0100,
                        (outs GPR:$rc), (ins GPR:$ra, GPR:$rb),
                        "mod\t$rc, $ra, $rb",
                        [(set GPR:$rc, (srem GPR:$ra, GPR:$rb))]>;

// and: Bitwise AND (rc = ra & rb) - opcode 10000, sel=0101
def AND : CGP1InstArith<0b10000, 0b0101,
                        (outs GPR:$rc), (ins GPR:$ra, GPR:$rb),
                        "and\t$rc, $ra, $rb",
                        [(set GPR:$rc, (and GPR:$ra, GPR:$rb))]>;

// or: Bitwise OR (rc = ra | rb) - opcode 10000, sel=0110
def OR : CGP1InstArith<0b10000, 0b0110,
                       (outs GPR:$rc), (ins GPR:$ra, GPR:$rb),
                       "or\t$rc, $ra, $rb",
                       [(set GPR:$rc, (or GPR:$ra, GPR:$rb))]>;

// xor: Bitwise XOR (rc = ra ^ rb) - opcode 10000, sel=0111
def XOR : CGP1InstArith<0b10000, 0b0111,
                        (outs GPR:$rc), (ins GPR:$ra, GPR:$rb),
                        "xor\t$rc, $ra, $rb",
                        [(set GPR:$rc, (xor GPR:$ra, GPR:$rb))]>;

// shl: Shift left (rc = ra << rb) - opcode 10000, sel=1000
def SHL : CGP1InstArith<0b10000, 0b1000,
                        (outs GPR:$rc), (ins GPR:$ra, GPR:$rb),
                        "shl\t$rc, $ra, $rb",
                        [(set GPR:$rc, (shl GPR:$ra, GPR:$rb))]>;

// shr: Shift right logical (rc = ra >> rb) - opcode 10000, sel=1001
def SHR : CGP1InstArith<0b10000, 0b1001,
                        (outs GPR:$rc), (ins GPR:$ra, GPR:$rb),
                        "shr\t$rc, $ra, $rb",
                        [(set GPR:$rc, (srl GPR:$ra, GPR:$rb))]>;

// shra: Shift right arithmetic - opcode 10000, sel=1010
def SHRA : CGP1InstArith<0b10000, 0b1010,
                         (outs GPR:$rc), (ins GPR:$ra, GPR:$rb),
                         "shra\t$rc, $ra, $rb",
                         [(set GPR:$rc, (sra GPR:$ra, GPR:$rb))]>;

//===----------------------------------------------------------------------===//
// Memory Instructions (opcodes 01011-10011)
//===----------------------------------------------------------------------===//

// load_addr: Load address (rc = ra + offset) - opcode 10001
def LOAD_ADDR : CGP1InstLoad<0b10001,
                              (outs GPR:$rc), (ins mem:$addr),
                              "load_addr\t$rc, $addr", []>;

// store_addr: Store address - opcode 10010
def STORE_ADDR : CGP1InstStore<0b10010,
                                (outs), (ins GPR:$rb, mem:$addr),
                                "store_addr\t$rb, $addr", []>;

// load: Load word from memory (rc = mem[ra]) - opcode 10011
// Pattern handles both i32 and f32 loads (unified register file)
def LOAD : CGP1InstLoad<0b10011,
                        (outs GPR:$rc), (ins mem:$addr),
                        "load\t$rc, $addr", []>;

// store: Store word to memory (mem[ra] = rb) - opcode 10100
def STORE : CGP1InstStore<0b10100,
                          (outs), (ins GPR:$rb, mem:$addr),
                          "store\t$addr, $rb", []>;

// Load/Store patterns for both i32 and f32
def : Pat<(i32 (load addr:$addr)), (LOAD addr:$addr)>;
def : Pat<(f32 (load addr:$addr)), (LOAD addr:$addr)>;
def : Pat<(store (i32 GPR:$rb), addr:$addr), (STORE GPR:$rb, addr:$addr)>;
def : Pat<(store (f32 GPR:$rb), addr:$addr), (STORE GPR:$rb, addr:$addr)>;

// push: Push to stack (store + decrement SP) - opcode 10100
let Defs = [R13], Uses = [R13] in
def PUSH : CGP1InstStore<0b10100,
                         (outs), (ins GPR:$rb),
                         "push\t$rb", [(CGP1execpush GPR:$rb)]>;

// pop: Pop from stack (increment SP + load) - opcode 10011
let Defs = [R13], Uses = [R13] in
def POP : CGP1InstLoad<0b10011,
                       (outs GPR:$rc), (ins),
                       "pop\t$rc", [(set GPR:$rc, (CGP1execpop))]>;

// d_load: Direct load - opcode 10011
def D_LOAD : CGP1InstLoad<0b10011,
                          (outs GPR:$rc), (ins mem:$addr),
                          "d_load\t$rc, $addr", []>;

// i_load: Indirect load - opcode 10011
def I_LOAD : CGP1InstLoad<0b10011,
                          (outs GPR:$rc), (ins mem:$addr),
                          "i_load\t$rc, $addr", []>;

// load_d: Load with decrement - opcode 10011
def LOAD_D : CGP1InstLoad<0b10011,
                          (outs GPR:$rc), (ins mem:$addr),
                          "load_d\t$rc, $addr", []>;

// load_i: Load with increment - opcode 10011
def LOAD_I : CGP1InstLoad<0b10011,
                          (outs GPR:$rc), (ins mem:$addr),
                          "load_i\t$rc, $addr", []>;

// d_store: Direct store - opcode 10100
def D_STORE : CGP1InstStore<0b10100,
                            (outs), (ins GPR:$rb, mem:$addr),
                            "d_store\t$addr, $rb", []>;

// i_store: Indirect store - opcode 10100
def I_STORE : CGP1InstStore<0b10100,
                            (outs), (ins GPR:$rb, mem:$addr),
                            "i_store\t$addr, $rb", []>;

// store_d: Store with decrement - opcode 10100
def STORE_D : CGP1InstStore<0b10100,
                            (outs), (ins GPR:$rb, mem:$addr),
                            "store_d\t$addr, $rb", []>;

// store_i: Store with increment - opcode 10100
def STORE_I : CGP1InstStore<0b10100,
                            (outs), (ins GPR:$rb, mem:$addr),
                            "store_i\t$addr, $rb", []>;

//===----------------------------------------------------------------------===//
// Control Flow Instructions (opcodes 10100-11001)
//===----------------------------------------------------------------------===//

// jump: Unconditional jump - opcode 10101
def JUMP : CGP1InstBranch<0b10101,
                          (outs), (ins brtarget:$target),
                          "jump\t$target",
                          [(br bb:$target)]>;

// goto: Goto - opcode 10110
def GOTO : CGP1InstBranch<0b10110,
                          (outs), (ins brtarget:$target),
                          "goto\t$target", []>;

// Conditional branches - compare register against zero and branch
// These use the condition field to encode the comparison type
let isBranch = 1, isTerminator = 1 in {
  // Branch if register is zero (BEQ with zero)
  def BRZ : CGP1Inst<(outs), (ins GPR:$rs, brtarget:$target),
                     "brz\t$rs, $target",
                     [(CGP1brz GPR:$rs, bb:$target)]> {
    let opcode = 0b10101;
  }
  
  // Branch if register is not zero (BNE with zero)  
  def BRNZ : CGP1Inst<(outs), (ins GPR:$rs, brtarget:$target),
                      "brnz\t$rs, $target",
                      [(CGP1brnz GPR:$rs, bb:$target)]> {
    let opcode = 0b10101;
  }
  
  // Branch if register is negative (less than zero)
  def BRLTZ : CGP1Inst<(outs), (ins GPR:$rs, brtarget:$target),
                       "brltz\t$rs, $target",
                       [(CGP1brltz GPR:$rs, bb:$target)]> {
    let opcode = 0b10101;
  }
  
  // Branch if register is greater than or equal to zero
  def BRGEZ : CGP1Inst<(outs), (ins GPR:$rs, brtarget:$target),
                       "brgez\t$rs, $target",
                       [(CGP1brgez GPR:$rs, bb:$target)]> {
    let opcode = 0b10101;
  }
}

// call: Function call (save LR, jump to target) - opcode 10110
def CALL : CGP1InstCall<0b10110,
                        (outs), (ins calltarget:$target),
                        "call\t$target", []>;

// rcall: Register call (call via register) - opcode 10101
def RCALL : CGP1InstCall<0b10101,
                         (outs), (ins GPR:$ra),
                         "rcall\t$ra", []>;

// ret: Return from function (jump to LR) - opcode 10111
def RET : CGP1InstReturn<0b10111,
                         (outs), (ins),
                         "ret", []>;

// stop_core: Stop core execution - opcode 11000
// Used for kernel termination (CGP1ISD::EndKernel)
let hasSideEffects = 1, isTerminator = 1, isReturn = 1, isBarrier = 1 in
def STOP_CORE : CGP1Inst<(outs), (ins), "stop_core", [(CGP1endkernel)]> {
  let opcode = 0b11000;
}

// SIMT Branch Pseudo Instruction
// This instruction handles divergent control flow by managing EXEC masks
// Operands: ConditionMask, TakenExec, FallThroughExec, TakenDest, FallThroughDest
// The reconvergence pass handles fall-through path EXEC mask updates
let isTerminator = 1, isBranch = 1, hasSideEffects = 1 in
def SIMT_BRANCH : PseudoCGP1Inst<(outs), 
                                 (ins GPR:$cond, GPR:$taken_exec, GPR:$fallthrough_exec, 
                                      brtarget:$taken, brtarget:$fallthrough),
                                 "# SIMT_BRANCH",
                                 [(CGP1simtbr GPR:$cond, GPR:$taken_exec, GPR:$fallthrough_exec, 
                                              bb:$taken, bb:$fallthrough)]>;

// irq: Interrupt request - opcode 11001
let hasSideEffects = 1 in
def IRQ : CGP1Inst<(outs), (ins i32imm:$vector), "irq\t$vector", []> {
  let opcode = 0b11001;
}

//===----------------------------------------------------------------------===//
// GPU Thread/Block Intrinsic Instructions (opcode 11010)
//===----------------------------------------------------------------------===//

// GPU register reads - mayLoad = 1 because they read from special GPU registers
let hasSideEffects = 0, mayLoad = 1, mayStore = 0 in {
  // Thread ID within block (sel field distinguishes x/y/z)
  def THREAD_ID_X : CGP1Inst<(outs GPR:$rd), (ins), "thread_id_x\t$rd",
                             [(set GPR:$rd, (int_cgp1_thread_id_x))]> {
    let opcode = 0b11010;
  }
  def THREAD_ID_Y : CGP1Inst<(outs GPR:$rd), (ins), "thread_id_y\t$rd",
                             [(set GPR:$rd, (int_cgp1_thread_id_y))]> {
    let opcode = 0b11010;
  }
  def THREAD_ID_Z : CGP1Inst<(outs GPR:$rd), (ins), "thread_id_z\t$rd",
                             [(set GPR:$rd, (int_cgp1_thread_id_z))]> {
    let opcode = 0b11010;
  }
  
  // Block ID within grid
  def BLOCK_ID_X : CGP1Inst<(outs GPR:$rd), (ins), "block_id_x\t$rd",
                            [(set GPR:$rd, (int_cgp1_block_id_x))]> {
    let opcode = 0b11010;
  }
  def BLOCK_ID_Y : CGP1Inst<(outs GPR:$rd), (ins), "block_id_y\t$rd",
                            [(set GPR:$rd, (int_cgp1_block_id_y))]> {
    let opcode = 0b11010;
  }
  def BLOCK_ID_Z : CGP1Inst<(outs GPR:$rd), (ins), "block_id_z\t$rd",
                            [(set GPR:$rd, (int_cgp1_block_id_z))]> {
    let opcode = 0b11010;
  }
  
  // Block dimensions
  def BLOCK_DIM_X : CGP1Inst<(outs GPR:$rd), (ins), "block_dim_x\t$rd",
                             [(set GPR:$rd, (int_cgp1_block_dim_x))]> {
    let opcode = 0b11010;
  }
  def BLOCK_DIM_Y : CGP1Inst<(outs GPR:$rd), (ins), "block_dim_y\t$rd",
                             [(set GPR:$rd, (int_cgp1_block_dim_y))]> {
    let opcode = 0b11010;
  }
  def BLOCK_DIM_Z : CGP1Inst<(outs GPR:$rd), (ins), "block_dim_z\t$rd",
                             [(set GPR:$rd, (int_cgp1_block_dim_z))]> {
    let opcode = 0b11010;
  }
  
  // Grid dimensions
  def GRID_DIM_X : CGP1Inst<(outs GPR:$rd), (ins), "grid_dim_x\t$rd",
                            [(set GPR:$rd, (int_cgp1_grid_dim_x))]> {
    let opcode = 0b11010;
  }
  def GRID_DIM_Y : CGP1Inst<(outs GPR:$rd), (ins), "grid_dim_y\t$rd",
                            [(set GPR:$rd, (int_cgp1_grid_dim_y))]> {
    let opcode = 0b11010;
  }
  def GRID_DIM_Z : CGP1Inst<(outs GPR:$rd), (ins), "grid_dim_z\t$rd",
                            [(set GPR:$rd, (int_cgp1_grid_dim_z))]> {
    let opcode = 0b11010;
  }
}

//===----------------------------------------------------------------------===//
// GPU Synchronization (opcode 11011)
//===----------------------------------------------------------------------===//

let hasSideEffects = 1, isBarrier = 1 in
def BARRIER : CGP1Inst<(outs), (ins), "barrier", [(CGP1barrier)]> {
  let opcode = 0b11011;
}

//===----------------------------------------------------------------------===//
// GPU Graphics Operations (opcode 11100)
//===----------------------------------------------------------------------===//

let hasSideEffects = 1, mayStore = 1 in {
  def WRITE_PIXEL : CGP1Inst<(outs), (ins GPR:$x, GPR:$y, GPR:$color),
                             "write_pixel\t$x, $y, $color",
                             [(int_cgp1_write_pixel GPR:$x, GPR:$y, GPR:$color)]> {
    let opcode = 0b11100;
  }
}

// Graphics ops with custom lowering - no store in pattern
let hasSideEffects = 1 in {
  def CLEAR_SCREEN : CGP1Inst<(outs), (ins),
                              "clear_screen",
                              [(CGP1clearscreen)]> {
    let opcode = 0b11100;
  }
  
  def DRAW_LINE : CGP1Inst<(outs), (ins),
                           "draw_line",
                           [(CGP1drawline)]> {
    let opcode = 0b11100;
  }
  
  def DRAW_TRIANGLE : CGP1Inst<(outs), (ins),
                               "draw_triangle",
                               [(CGP1drawtriangle)]> {
    let opcode = 0b11100;
  }
}

//===----------------------------------------------------------------------===//
// Pseudo Instructions
//===----------------------------------------------------------------------===//

// Pseudo return (matches CGP1RetSDN, expands to RET)
def PseudoRet : PseudoCGP1Inst<(outs), (ins), "ret",
                               [(CGP1RetSDN)]> {
  let isReturn = 1;
  let isTerminator = 1;
  let isBarrier = 1;
}

// Call sequence start/end for stack adjustment
def SDT_CGP1CallSeqStart : SDCallSeqStart<[SDTCisVT<0, i32>, SDTCisVT<1, i32>]>;
def SDT_CGP1CallSeqEnd : SDCallSeqEnd<[SDTCisVT<0, i32>, SDTCisVT<1, i32>]>;

def callseq_start : SDNode<"ISD::CALLSEQ_START", SDT_CGP1CallSeqStart,
                           [SDNPHasChain, SDNPOutGlue]>;
def callseq_end : SDNode<"ISD::CALLSEQ_END", SDT_CGP1CallSeqEnd,
                         [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue]>;

let Defs = [R13], Uses = [R13] in {
  def ADJCALLSTACKDOWN : PseudoCGP1Inst<(outs), (ins i32imm:$amt1, i32imm:$amt2),
                                        "# ADJCALLSTACKDOWN $amt1, $amt2",
                                        [(callseq_start timm:$amt1, timm:$amt2)]>;
  def ADJCALLSTACKUP : PseudoCGP1Inst<(outs), (ins i32imm:$amt1, i32imm:$amt2),
                                      "# ADJCALLSTACKUP $amt1, $amt2",
                                      [(callseq_end timm:$amt1, timm:$amt2)]>;
}

//===----------------------------------------------------------------------===//
// Pattern Matching
//===----------------------------------------------------------------------===//

// Call patterns
def : Pat<(CGP1call (i32 tglobaladdr:$target)), (CALL tglobaladdr:$target)>;
def : Pat<(CGP1call (i32 texternalsym:$target)), (CALL texternalsym:$target)>;
def : Pat<(CGP1call GPR:$ra), (RCALL GPR:$ra)>;

// Immediate patterns
def simm16imm : ImmLeaf<i32, [{ return isInt<16>(Imm); }]>;
def uimm16imm : ImmLeaf<i32, [{ return isUInt<16>(Imm); }]>;

//===----------------------------------------------------------------------===//
// Shared Memory Instructions (opcode 11101)
//===----------------------------------------------------------------------===//

let hasSideEffects = 0 in {
  // Load from shared memory
  let mayLoad = 1 in
  def SHARED_LOAD : CGP1Inst<(outs GPR:$rd), (ins GPR:$addr),
                             "shared_load\t$rd, $addr",
                             [(set GPR:$rd, (int_cgp1_shared_load GPR:$addr))]> {
    let opcode = 0b11101;
  }
  
  // Store to shared memory
  let mayStore = 1 in
  def SHARED_STORE : CGP1Inst<(outs), (ins GPR:$addr, GPR:$val),
                              "shared_store\t$addr, $val",
                              [(int_cgp1_shared_store GPR:$addr, GPR:$val)]> {
    let opcode = 0b11101;
  }
}

//===----------------------------------------------------------------------===//
// Atomic Instructions (opcode 11110)
//===----------------------------------------------------------------------===//

let hasSideEffects = 1, mayLoad = 1, mayStore = 1 in {
  def ATOMIC_ADD : CGP1Inst<(outs GPR:$rd), (ins GPR:$addr, GPR:$val),
                            "atomic_add\t$rd, $addr, $val",
                            [(set GPR:$rd, (CGP1atomic_add GPR:$addr, GPR:$val))]> {
    let opcode = 0b11110;
  }
  
  def ATOMIC_CAS : CGP1Inst<(outs GPR:$rd), (ins GPR:$addr, GPR:$cmp, GPR:$val),
                            "atomic_cas\t$rd, $addr, $cmp, $val",
                            [(set GPR:$rd, (CGP1atomic_cas GPR:$addr, GPR:$cmp, GPR:$val))]> {
    let opcode = 0b11110;
  }
  
  def ATOMIC_MAX : CGP1Inst<(outs GPR:$rd), (ins GPR:$addr, GPR:$val),
                            "atomic_max\t$rd, $addr, $val",
                            [(set GPR:$rd, (CGP1atomic_max GPR:$addr, GPR:$val))]> {
    let opcode = 0b11110;
  }
  
  def ATOMIC_MIN : CGP1Inst<(outs GPR:$rd), (ins GPR:$addr, GPR:$val),
                            "atomic_min\t$rd, $addr, $val",
                            [(set GPR:$rd, (CGP1atomic_min GPR:$addr, GPR:$val))]> {
    let opcode = 0b11110;
  }
  
  def ATOMIC_EXCH : CGP1Inst<(outs GPR:$rd), (ins GPR:$addr, GPR:$val),
                             "atomic_exch\t$rd, $addr, $val",
                             [(set GPR:$rd, (CGP1atomic_exch GPR:$addr, GPR:$val))]> {
    let opcode = 0b11110;
  }
}

//===----------------------------------------------------------------------===//
// Warp-Level Instructions (opcode 11111)
//===----------------------------------------------------------------------===//

let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in {
  // Warp size (constant 32)
  def WARP_SIZE : CGP1Inst<(outs GPR:$rd), (ins), "warp_size\t$rd",
                           [(set GPR:$rd, (CGP1warp_size))]> {
    let opcode = 0b11111;
  }
  
  // Lane ID within warp (reads from special register)
  def LANE_ID : CGP1Inst<(outs GPR:$rd), (ins), "lane_id\t$rd",
                         [(set GPR:$rd, (CGP1lane_id))]> {
    let opcode = 0b11111;
  }
}

let hasSideEffects = 0 in {
  // Warp vote: all
  def WARP_ALL : CGP1Inst<(outs GPR:$rd), (ins GPR:$pred),
                          "warp_all\t$rd, $pred",
                          [(set GPR:$rd, (CGP1warp_all GPR:$pred))]> {
    let opcode = 0b11111;
  }
  
  // Warp vote: any
  def WARP_ANY : CGP1Inst<(outs GPR:$rd), (ins GPR:$pred),
                          "warp_any\t$rd, $pred",
                          [(set GPR:$rd, (CGP1warp_any GPR:$pred))]> {
    let opcode = 0b11111;
  }
  
  // Warp ballot
  def WARP_BALLOT : CGP1Inst<(outs GPR:$rd), (ins GPR:$pred),
                             "warp_ballot\t$rd, $pred",
                             [(set GPR:$rd, (CGP1warp_ballot GPR:$pred))]> {
    let opcode = 0b11111;
  }
  
  // Warp shuffle
  def WARP_SHUFFLE : CGP1Inst<(outs GPR:$rd), (ins GPR:$val, GPR:$lane),
                              "warp_shuffle\t$rd, $val, $lane",
                              [(set GPR:$rd, (CGP1warp_shuffle GPR:$val, GPR:$lane))]> {
    let opcode = 0b11111;
  }
}

//===----------------------------------------------------------------------===//
// Fast Math Instructions (reusing FP opcodes with different sel)
//===----------------------------------------------------------------------===//

// Fast reciprocal (1/x) - use fdiv opcode with sel=1
def FRCP : CGP1InstUnary<0b00110, 0b0001,
                         (outs GPR:$rc), (ins GPR:$ra),
                         "frcp\t$rc, $ra",
                         [(set GPR:$rc, (int_cgp1_rcp GPR:$ra))]>;

// Fast reciprocal square root - use fdiv opcode with sel=2
def FRSQRT : CGP1InstUnary<0b00110, 0b0010,
                           (outs GPR:$rc), (ins GPR:$ra),
                           "frsqrt\t$rc, $ra",
                           [(set GPR:$rc, (int_cgp1_rsqrt GPR:$ra))]>;

// Fast sine - use trunc/round opcode with sel=4
def FSIN : CGP1InstUnary<0b01000, 0b0100,
                         (outs GPR:$rc), (ins GPR:$ra),
                         "fsin\t$rc, $ra",
                         [(set GPR:$rc, (int_cgp1_sin GPR:$ra))]>;

// Fast cosine - use trunc/round opcode with sel=5
def FCOS : CGP1InstUnary<0b01000, 0b0101,
                         (outs GPR:$rc), (ins GPR:$ra),
                         "fcos\t$rc, $ra",
                         [(set GPR:$rc, (int_cgp1_cos GPR:$ra))]>;

// Fast exp - use trunc/round opcode with sel=6
def FEXP : CGP1InstUnary<0b01000, 0b0110,
                         (outs GPR:$rc), (ins GPR:$ra),
                         "fexp\t$rc, $ra",
                         [(set GPR:$rc, (int_cgp1_exp GPR:$ra))]>;

// Fast log - use trunc/round opcode with sel=7
def FLOG : CGP1InstUnary<0b01000, 0b0111,
                         (outs GPR:$rc), (ins GPR:$ra),
                         "flog\t$rc, $ra",
                         [(set GPR:$rc, (int_cgp1_log GPR:$ra))]>;

//===----------------------------------------------------------------------===//
// OpenGL-like Graphics Instructions
//===----------------------------------------------------------------------===//

let hasSideEffects = 1 in {
  // Set viewport
  def SET_VIEWPORT : CGP1Inst<(outs), (ins GPR:$x, GPR:$y, GPR:$w, GPR:$h),
                              "set_viewport\t$x, $y, $w, $h",
                              [(int_cgp1_set_viewport GPR:$x, GPR:$y, GPR:$w, GPR:$h)]> {
    let opcode = 0b11100;
  }
  
  // Set current color
  def SET_COLOR : CGP1Inst<(outs), (ins GPR:$color),
                           "set_color\t$color",
                           [(int_cgp1_set_color GPR:$color)]> {
    let opcode = 0b11100;
  }
  
  // Set depth
  def SET_DEPTH : CGP1Inst<(outs), (ins GPR:$depth),
                           "set_depth\t$depth",
                           [(int_cgp1_set_depth GPR:$depth)]> {
    let opcode = 0b11100;
  }
  
  // Begin primitive
  def GFX_BEGIN : CGP1Inst<(outs), (ins GPR:$mode),
                           "gfx_begin\t$mode",
                           [(int_cgp1_begin GPR:$mode)]> {
    let opcode = 0b11100;
  }
  
  // End primitive
  def GFX_END : CGP1Inst<(outs), (ins),
                         "gfx_end",
                         [(int_cgp1_end)]> {
    let opcode = 0b11100;
  }
  
  // Vertex 2D integer
  def VERTEX2I : CGP1Inst<(outs), (ins GPR:$x, GPR:$y),
                          "vertex2i\t$x, $y",
                          [(int_cgp1_vertex2i GPR:$x, GPR:$y)]> {
    let opcode = 0b11100;
  }
  
  // Vertex 2D float
  def VERTEX2F : CGP1Inst<(outs), (ins GPR:$x, GPR:$y),
                          "vertex2f\t$x, $y",
                          [(int_cgp1_vertex2f GPR:$x, GPR:$y)]> {
    let opcode = 0b11100;
  }
  
  // Vertex 3D float
  def VERTEX3F : CGP1Inst<(outs), (ins GPR:$x, GPR:$y, GPR:$z),
                          "vertex3f\t$x, $y, $z",
                          [(int_cgp1_vertex3f GPR:$x, GPR:$y, GPR:$z)]> {
    let opcode = 0b11100;
  }
  
  // Draw rectangle
  def DRAW_RECT : CGP1Inst<(outs), (ins GPR:$x, GPR:$y, GPR:$w, GPR:$h),
                           "draw_rect\t$x, $y, $w, $h",
                           [(int_cgp1_draw_rect GPR:$x, GPR:$y, GPR:$w, GPR:$h)]> {
    let opcode = 0b11100;
  }
  
  // Fill rectangle
  def FILL_RECT : CGP1Inst<(outs), (ins GPR:$x, GPR:$y, GPR:$w, GPR:$h),
                           "fill_rect\t$x, $y, $w, $h",
                           [(int_cgp1_fill_rect GPR:$x, GPR:$y, GPR:$w, GPR:$h)]> {
    let opcode = 0b11100;
  }
  
  // Draw circle
  def DRAW_CIRCLE : CGP1Inst<(outs), (ins GPR:$cx, GPR:$cy, GPR:$r),
                             "draw_circle\t$cx, $cy, $r",
                             [(int_cgp1_draw_circle GPR:$cx, GPR:$cy, GPR:$r)]> {
    let opcode = 0b11100;
  }
  
  // Fill circle
  def FILL_CIRCLE : CGP1Inst<(outs), (ins GPR:$cx, GPR:$cy, GPR:$r),
                             "fill_circle\t$cx, $cy, $r",
                             [(int_cgp1_fill_circle GPR:$cx, GPR:$cy, GPR:$r)]> {
    let opcode = 0b11100;
  }
  
  // Swap buffers
  def SWAP_BUFFERS : CGP1Inst<(outs), (ins),
                              "swap_buffers",
                              [(int_cgp1_swap_buffers)]> {
    let opcode = 0b11100;
  }
  
  // Set blend mode
  def SET_BLEND_MODE : CGP1Inst<(outs), (ins GPR:$mode),
                                "set_blend_mode\t$mode",
                                [(int_cgp1_set_blend_mode GPR:$mode)]> {
    let opcode = 0b11100;
  }
}

// Read pixel (has output)
let hasSideEffects = 0, mayLoad = 1 in {
  def READ_PIXEL : CGP1Inst<(outs GPR:$color), (ins GPR:$x, GPR:$y),
                            "read_pixel\t$color, $x, $y",
                            [(set GPR:$color, (int_cgp1_read_pixel GPR:$x, GPR:$y))]> {
    let opcode = 0b11100;
  }
}

//===----------------------------------------------------------------------===//
// Texture Instructions
//===----------------------------------------------------------------------===//

let hasSideEffects = 1 in {
  // Bind texture
  def BIND_TEXTURE : CGP1Inst<(outs), (ins GPR:$texid),
                              "bind_texture\t$texid",
                              [(int_cgp1_bind_texture GPR:$texid)]> {
    let opcode = 0b11100;
  }
  
  // Set texture coordinates
  def TEXCOORD2F : CGP1Inst<(outs), (ins GPR:$u, GPR:$v),
                            "texcoord2f\t$u, $v",
                            [(int_cgp1_texcoord2f GPR:$u, GPR:$v)]> {
    let opcode = 0b11100;
  }
}

// Sample texture (has output)
let hasSideEffects = 0, mayLoad = 1 in {
  def TEX2D : CGP1Inst<(outs GPR:$color), (ins GPR:$u, GPR:$v),
                       "tex2d\t$color, $u, $v",
                       [(set GPR:$color, (int_cgp1_tex2d GPR:$u, GPR:$v))]> {
    let opcode = 0b11100;
  }
}

//===----------------------------------------------------------------------===//
// Matrix Instructions
//===----------------------------------------------------------------------===//

let hasSideEffects = 1 in {
  // Load identity matrix
  def LOAD_IDENTITY : CGP1Inst<(outs), (ins),
                               "load_identity",
                               [(int_cgp1_load_identity)]> {
    let opcode = 0b11100;
  }
  
  // Translate
  def MTX_TRANSLATE : CGP1Inst<(outs), (ins GPR:$x, GPR:$y, GPR:$z),
                               "translate\t$x, $y, $z",
                               [(int_cgp1_translate GPR:$x, GPR:$y, GPR:$z)]> {
    let opcode = 0b11100;
  }
  
  // Scale
  def MTX_SCALE : CGP1Inst<(outs), (ins GPR:$x, GPR:$y, GPR:$z),
                           "scale\t$x, $y, $z",
                           [(int_cgp1_scale GPR:$x, GPR:$y, GPR:$z)]> {
    let opcode = 0b11100;
  }
  
  // Rotate Z
  def MTX_ROTATE_Z : CGP1Inst<(outs), (ins GPR:$angle),
                              "rotate_z\t$angle",
                              [(int_cgp1_rotate_z GPR:$angle)]> {
    let opcode = 0b11100;
  }
}


