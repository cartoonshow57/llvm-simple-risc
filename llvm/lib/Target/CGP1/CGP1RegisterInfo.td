//===----------------------------------------------------------------------===//
// CGP1 GPU Register Definitions
// 
// The CGP1 GPU has 16 unified registers (r0-r15) that can hold both
// integer and floating-point values. Register fields are 4-bit encoded.
//
// Register Conventions:
//   r0       : Zero register (always 0) / General purpose
//   r1-r3    : Function arguments / Return values
//   r4-r11   : General purpose (caller-saved temporaries)
//   r12      : Frame pointer (FP)
//   r13      : Stack pointer (SP)
//   r14      : Link register (LR) - holds return address
//   r15      : Reserved / PC proxy
//===----------------------------------------------------------------------===//

// Base register class with 4-bit encoding
class CGP1Reg<bits<4> Enc, string n> : Register<n> {
  let Namespace = "CGP1";
  let HWEncoding{3-0} = Enc;
}

// Define all 16 general-purpose registers
let Namespace = "CGP1" in {
  def R0  : CGP1Reg<0,  "r0">;
  def R1  : CGP1Reg<1,  "r1">;
  def R2  : CGP1Reg<2,  "r2">;
  def R3  : CGP1Reg<3,  "r3">;
  def R4  : CGP1Reg<4,  "r4">;
  def R5  : CGP1Reg<5,  "r5">;
  def R6  : CGP1Reg<6,  "r6">;
  def R7  : CGP1Reg<7,  "r7">;
  def R8  : CGP1Reg<8,  "r8">;
  def R9  : CGP1Reg<9,  "r9">;
  def R10 : CGP1Reg<10, "r10">;
  def R11 : CGP1Reg<11, "r11">;
  def R12 : CGP1Reg<12, "r12">;  // Frame Pointer (FP)
  def R13 : CGP1Reg<13, "r13">;  // Stack Pointer (SP)
  def R14 : CGP1Reg<14, "r14">;  // Link Register (LR)
  def R15 : CGP1Reg<15, "r15">;  // Reserved
}

// Aliases for special registers
def SP : CGP1Reg<13, "sp">;
def LR : CGP1Reg<14, "lr">;
def FP : CGP1Reg<12, "fp">;

//===----------------------------------------------------------------------===//
// Register Classes
//===----------------------------------------------------------------------===//

// GPR - General Purpose Register class (unified for int and float)
// All 16 registers can hold i32 or f32 values
def GPR : RegisterClass<"CGP1", [i32, f32], 32,
  (add
   // General purpose / arguments / return values
   R0, R1, R2, R3,
   // Caller-saved temporaries
   R4, R5, R6, R7, R8, R9, R10, R11,
   // Frame pointer
   R12,
   // Stack pointer
   R13,
   // Link register
   R14,
   // Reserved
   R15
  )>;

// Subset without SP/LR/R15 for general allocation
def GPRnoSP : RegisterClass<"CGP1", [i32, f32], 32,
  (sub GPR, R13, R14, R15)>;

// For backward compatibility - alias GPR32 to GPR
def GPR32 : RegisterClass<"CGP1", [i32], 32,
  (add R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15)>;

// FPR32 - Same registers used for floating-point
def FPR32 : RegisterClass<"CGP1", [f32], 32,
  (add R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15)>;

// Non-allocatable register classes for special purposes
let isAllocatable = false in {
  def SPReg : RegisterClass<"CGP1", [i32], 32, (add R13)>;
  def LRReg : RegisterClass<"CGP1", [i32], 32, (add R14)>;
}
