//===-- SimpleRISCRegisterInfo.td - Registers -------------*- tablegen -*-===//
//
// This file describes the SimpleRISC register file.
//
//===----------------------------------------------------------------------===//

class SimpleRISCReg<string n> : Register<n>;

// Define the 8 General Purpose Registers (GPRs)
def R0 : SimpleRISCReg<"r0">;
def R1 : SimpleRISCReg<"r1">;
def R2 : SimpleRISCReg<"r2">;
def R3 : SimpleRISCReg<"r3">;
def R4 : SimpleRISCReg<"r4">;
def R5 : SimpleRISCReg<"r5">;
def R6 : SimpleRISCReg<"r6">;
def R7 : SimpleRISCReg<"r7">;

// Define the RegisterClass that groups all our GPRs.
def GPR : RegisterClass<"SimpleRISC", [i32], 32, (add
  R0, R1, R2, R3, R4, R5, R6, R7
)>;
