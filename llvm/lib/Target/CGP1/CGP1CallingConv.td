//===----------------------------------------------------------------------===//
// CGP1 GPU Calling Conventions
//
// This file defines calling conventions for the CGP1 GPU:
// 1. CGP1_Kernel - For GPU kernel entry points (invoked by hardware)
// 2. CGP1_Device - For device functions (called from kernels/other functions)
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// CGP1 Kernel Calling Convention
//
// Kernel entry points are invoked by the GPU hardware dispatcher.
// - Arguments are passed via a kernel argument buffer in memory
// - No return value (kernels don't return, they terminate with STOP_CORE)
// - Thread/block IDs are available via special registers/intrinsics
//===----------------------------------------------------------------------===//

def CC_CGP1_Kernel : CallingConv<[
  // Kernel arguments are passed in memory (argument buffer)
  // The first few can be promoted to registers for efficiency
  CCIfType<[i32, f32], CCAssignToReg<[R1, R2, R3, R4, R5, R6, R7, R8]>>,
  
  // Remaining arguments go to stack (kernel argument buffer)
  CCIfType<[i32, f32], CCAssignToStack<4, 4>>
]>;

// Kernel return - kernels don't return values, they terminate
def RetCC_CGP1_Kernel : CallingConv<[
  // No return values for kernels
]>;

//===----------------------------------------------------------------------===//
// CGP1 Device Function Calling Convention
//
// Device functions are called from kernels or other device functions.
// - Arguments passed in registers R1-R8, then stack
// - Return values in R1 (and R2 for 64-bit)
// - R13 (SP), R14 (LR), R15 reserved
// - Callee-saved: R9-R12
// - Caller-saved: R1-R8
//===----------------------------------------------------------------------===//

def CC_CGP1_Device : CallingConv<[
  // Promote i8/i16 to i32
  CCIfType<[i8, i16], CCPromoteToType<i32>>,
  
  // Pass first 8 arguments in registers
  CCIfType<[i32, f32], CCAssignToReg<[R1, R2, R3, R4, R5, R6, R7, R8]>>,
  
  // Remaining arguments on stack, 4-byte aligned
  CCIfType<[i32, f32], CCAssignToStack<4, 4>>
]>;

// Device function return convention
def RetCC_CGP1_Device : CallingConv<[
  // Return i32/f32 in R1
  CCIfType<[i32, f32], CCAssignToReg<[R1]>>,
  
  // For 64-bit returns (if needed), use R1:R2
  CCIfType<[i64], CCAssignToReg<[R1, R2]>>
]>;

//===----------------------------------------------------------------------===//
// Callee-Saved Registers
//
// These registers must be preserved across function calls.
// Kernels don't need to save anything (they're entry points).
// Device functions must preserve R9-R12, R13 (SP), R14 (LR).
//===----------------------------------------------------------------------===//

def CSR_CGP1_Kernel : CalleeSavedRegs<(add)>;  // Kernels save nothing

def CSR_CGP1_Device : CalleeSavedRegs<(add R9, R10, R11, R12)>;  // FP, SP, LR handled separately

