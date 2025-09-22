//===- CGP1RegisterInfo.td - CGP1 Register defs ------------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//  Declarations that describe the CGP1 register file
//===----------------------------------------------------------------------===//

// Registers are identified with 5-bit ID numbers.
class CGP1Reg<bits<16> num, string n, list<Register> subregs = [],
               list<string> altNames = []> : Register<n, altNames> {
  field bits<5> Num;
  let HWEncoding = num;
  let Namespace = "CGP1";
  let SubRegs = subregs;
}

// Base class for floating-point registers
class CGP1FReg<bits<16> num, string n> : Register<n> {
  let HWEncoding = num;
  let Namespace = "CGP1";
}

// Base class for Vector Registers
class CGP1VReg<bits<16> num, string n> : Register<n> {
  let HWEncoding = num;
  let Namespace = "CGP1";
}

let Namespace = "CGP1" in {
  def sub_32 : SubRegIndex<32>;
}

// Defining Integer registers
foreach i = 0-31 in {
  def R#i : CGP1Reg<i, "r"#i>, DwarfRegNum<[i]>;
}

// Defining Floating registers
foreach i = 0-31 in {
  def F#i : CGP1FReg<i, "f" # i>, DwarfRegNum<[!add(i, 32)]>;
}

// Defining Vector Registers
// Defining Vector Registers
foreach i = 0-31 in {
  def V#i : CGP1VReg<i, "v" # i>, DwarfRegNum<[!add(i, 64)]>;
}


// Register aliases
let SubRegIndices = [sub_32] in {
  def PC  : CGP1Reg< 2,  "pc",  [R2]>,  DwarfRegAlias<R2>;
  def SR  : CGP1Reg< 3,  "sw",  [R3]>,  DwarfRegAlias<R3>; // Status Register or Status Word in assembly
  def SP  : CGP1Reg< 4,  "sp",  [R4]>,  DwarfRegAlias<R4>;
  def FP  : CGP1Reg< 5,  "fp",  [R5]>,  DwarfRegAlias<R5>;
  def RV  : CGP1Reg< 8,  "rv",  [R8]>,  DwarfRegAlias<R8>;
  def RR1 : CGP1Reg<10, "rr1", [R10]>, DwarfRegAlias<R10>;
  def RR2 : CGP1Reg<11, "rr2", [R11]>, DwarfRegAlias<R11>;
  def RCA : CGP1Reg<15, "rca", [R15]>, DwarfRegAlias<R15>;
}

// Define a status register to capture the dependencies between the set flag
// and setcc instructions
// def SR : CGP1Reg< 0, "sw">; Moved in the alias 

// Register classes.
def GPR : RegisterClass<"CGP1", [i32], 32,
    (add // General purpose temporaries
     R9, R12, R13, R14, R16, R17,

     // Callee-saved registers
     (sequence "R%i", 20, 31),

     // Argument registers (caller-saved)
     R6, R7, R18, R19,

     // Other special/caller-saved registers
     R15, // constant addresses (RCA)
     R10, R11, // programmer controlled (RR1, RR2)
     R8,  // return value (RV)
     R5,  // frame pointer (FP)
     R4,  // stack pointer (SP)
     R3,  // status word (SR)
     R2,  // program counter (PC)
     R1,  // all 1s (0xffffffff)
     R0   // constant 0
    )>;

// Condition code register class
def CCR : RegisterClass<"CGP1", [i32], 32, (add SR)> {
  let CopyCost = -1; // Don't allow copying of status registers
  let isAllocatable = 0;
}

// Grouping all floating point registers
def FPR : RegisterClass<"CGP1", [f32], 32, (add (sequence "F%u", 0, 31))>;

// Grouping all the vector registers
def VPR : RegisterClass<"CGP1", [v4f32], 128, (add (sequence "V%u", 0, 31))>;

// --- Calling Convention ---
// Lists the registers that must be saved by a function before it can use them.
def CalleeSavedRegs : CalleeSavedRegs<(add
  // Preserved General Purpose Registers
  (sequence "R%u", 20, 31),

  // Preserved Floating-Point Registers
  (sequence "F%u", 20, 31),

  // Preserved Vector Registers
  (sequence "V%u", 20, 31)
)>;
