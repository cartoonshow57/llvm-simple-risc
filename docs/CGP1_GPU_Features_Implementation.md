# CGP1 GPU Backend: Feature Implementation Documentation

This document describes the major GPU features implemented in the CGP1 backend, explaining what each feature means for GPU computing and how it was implemented in LLVM.

---

## Table of Contents

1. [GPU Execution Model: Threads, Blocks, and Grids](#1-gpu-execution-model-threads-blocks-and-grids)
2. [SIMT Execution and Divergent Control Flow](#2-simt-execution-and-divergent-control-flow)
3. [Memory Spaces: Global, Shared, and Local](#3-memory-spaces-global-shared-and-local)
4. [Synchronization: Barriers](#4-synchronization-barriers)
5. [Atomic Operations](#5-atomic-operations)
6. [Warp-Level Primitives](#6-warp-level-primitives)
7. [Graphics Operations](#7-graphics-operations)
8. [Kernel vs Device Functions](#8-kernel-vs-device-functions)
9. [Fast Math Intrinsics](#9-fast-math-intrinsics)

---

## 1. GPU Execution Model: Threads, Blocks, and Grids

### What It Is

GPUs execute thousands of threads in parallel. The execution model is organized hierarchically:

- **Thread**: The smallest execution unit (like a CPU thread)
- **Block**: A group of threads that can synchronize and share memory
- **Grid**: A collection of blocks that execute the same kernel

Each thread needs to know its position in this hierarchy to compute on different data elements.

### Why It Matters

This hierarchical model allows:
- **Data parallelism**: Each thread processes different data (e.g., different pixels)
- **Cooperation**: Threads in a block can share fast on-chip memory
- **Scalability**: The same kernel code runs on grids of any size

### Implementation in CGP1

#### Frontend: Intrinsics and Builtins

**Files Created/Modified:**
- `llvm/include/llvm/IR/IntrinsicsCGP1.td` - LLVM intrinsic definitions
- `clang/include/clang/Basic/BuiltinsCGP1.def` - Clang builtin definitions
- `clang/lib/CodeGen/CGBuiltin.cpp` - Code generation for builtins

**Intrinsics Defined:**
```tablegen
// Thread ID within a block (0 to block_dim - 1)
def int_cgp1_thread_id_x : Intrinsic<[llvm_i32_ty], [], [...]>;
def int_cgp1_thread_id_y : Intrinsic<[llvm_i32_ty], [], [...]>;
def int_cgp1_thread_id_z : Intrinsic<[llvm_i32_ty], [], [...]>;

// Block ID within the grid (0 to grid_dim - 1)
def int_cgp1_block_id_x : Intrinsic<[llvm_i32_ty], [], [...]>;
def int_cgp1_block_id_y : Intrinsic<[llvm_i32_ty], [], [...]>;
def int_cgp1_block_id_z : Intrinsic<[llvm_i32_ty], [], [...]>;

// Block dimensions (number of threads per block)
def int_cgp1_block_dim_x : Intrinsic<[llvm_i32_ty], [], [...]>;
def int_cgp1_block_dim_y : Intrinsic<[llvm_i32_ty], [], [...]>;
def int_cgp1_block_dim_z : Intrinsic<[llvm_i32_ty], [], [...]>;

// Grid dimensions (number of blocks in the grid)
def int_cgp1_grid_dim_x : Intrinsic<[llvm_i32_ty], [], [...]>;
def int_cgp1_grid_dim_y : Intrinsic<[llvm_i32_ty], [], [...]>;
def int_cgp1_grid_dim_z : Intrinsic<[llvm_i32_ty], [], [...]>;
```

**Clang Builtins:**
```cpp
// In BuiltinsCGP1.def
BUILTIN(__builtin_cgp1_thread_id_x, "i", "nc")
BUILTIN(__builtin_cgp1_block_id_x, "i", "nc")
BUILTIN(__builtin_cgp1_block_dim_x, "i", "nc")
BUILTIN(__builtin_cgp1_grid_dim_x, "i", "nc")
// ... (y and z variants)
```

#### Backend: Lowering to Registers

**Files Modified:**
- `llvm/lib/Target/CGP1/CGP1ISelLowering.cpp` - Intrinsic lowering

**Key Implementation:**

In `CGP1ISelLowering::lowerIntrinsic()`, thread/block IDs are mapped to physical registers that are set up by the GPU runtime:

```cpp
case Intrinsic::cgp1_thread_id_x: {
  // Read from R14 (LR) which holds thread ID in kernels
  MachineFunction &MF = DAG.getMachineFunction();
  MachineRegisterInfo &RegInfo = MF.getRegInfo();
  Register ThreadIDReg = 0;
  
  for (const auto &LI : RegInfo.liveins()) {
    if (LI.first == CGP1::R14) {
      ThreadIDReg = LI.second;
      break;
    }
  }
  
  if (ThreadIDReg)
    return DAG.getCopyFromReg(Chain, DL, ThreadIDReg, VT);
  // ...
}
```

**Register Assignment:**
- `R14` (Link Register): Thread ID (threadIdx.x)
- `R11`: Block ID (blockIdx.x)
- `R10`: Block Dimension (blockDim.x)
- `R9`: Grid Dimension (gridDim.x)

These registers are set up as live-ins in kernel functions (see Section 8).

#### Instruction Set: GPU Register Reads

**Files Modified:**
- `llvm/lib/Target/CGP1/CGP1InstrInfo.td` - Instruction definitions
- `llvm/lib/Target/CGP1/MCTargetDesc/CGP1MCCodeEmitter.cpp` - Binary encoding

**Instructions Defined:**
```tablegen
// Opcode 0b11010 with sel field distinguishing x/y/z
def THREAD_ID_X : CGP1Inst<(outs GPR:$rd), (ins), "thread_id_x\t$rd",
                            [(set GPR:$rd, (int_cgp1_thread_id_x))]>;
def BLOCK_ID_X : CGP1Inst<(outs GPR:$rd), (ins), "block_id_x\t$rd",
                          [(set GPR:$rd, (int_cgp1_block_id_x))]>;
// ... (y and z variants)
```

**Binary Encoding:**
```cpp
// In CGP1MCCodeEmitter.cpp
case CGP1::THREAD_ID_X:
  Binary = encodeInstr(0b11010, COND_AL, 0, 0b0000, 0, 0, getReg(0));
  break;
case CGP1::BLOCK_ID_X:
  Binary = encodeInstr(0b11010, COND_AL, 0, 0b0011, 0, 0, getReg(0));
  break;
```

---

## 2. SIMT Execution and Divergent Control Flow

### What It Is

**SIMT (Single Instruction, Multiple Threads)** is the execution model where multiple threads execute the same instruction stream, but each thread can take different paths through branches. When threads diverge (some take one branch, others take another), the GPU must:

1. **Track active threads** using an EXEC mask (bitmask indicating which threads are active)
2. **Execute both paths** sequentially, masking out inactive threads
3. **Reconverge** at merge points, restoring the full EXEC mask

### Why It Matters

Without SIMT divergence handling:
- Divergent branches would cause incorrect execution
- Performance would degrade significantly (all threads execute all paths)
- Many GPU algorithms wouldn't work correctly

### Implementation in CGP1

#### EXEC Mask Model

**Files Created/Modified:**
- `llvm/lib/Target/CGP1/CGP1ISelLowering.cpp` - EXEC mask intrinsics lowering
- `llvm/lib/Target/CGP1/CGP1FrameLowering.cpp` - EXEC mask initialization
- `llvm/lib/Target/CGP1/CGP1SIMTReconvergence.cpp` - Reconvergence pass

**EXEC Mask Register:**
- `R15` is reserved as the EXEC mask register
- Each bit represents one thread in a warp (32 threads = 32 bits)
- `1` = thread is active, `0` = thread is inactive

**Intrinsics:**
```tablegen
// In IntrinsicsCGP1.td
def int_cgp1_exec_get : Intrinsic<[llvm_i32_ty], [], [...]>;  // Read EXEC
def int_cgp1_exec_set : Intrinsic<[], [llvm_i32_ty], [...]>;  // Write EXEC
def int_cgp1_exec_push : Intrinsic<[], [], [...]>;            // Push to stack
def int_cgp1_exec_pop : Intrinsic<[], [], [...]>;              // Pop from stack
def int_cgp1_exec_and : Intrinsic<[], [llvm_i32_ty], [...]>;  // EXEC &= mask
```

#### EXEC Mask Initialization

**File:** `llvm/lib/Target/CGP1/CGP1FrameLowering.cpp`

**Code:**
```cpp
void CGP1FrameLowering::emitPrologue(MachineFunction &MF,
                                     MachineBasicBlock &MBB) const {
  const Function &F = MF.getFunction();

  // Minimal SIMT model: initialize EXEC mask to all-ones for kernels.
  if (F.getCallingConv() == CallingConv::SPIR_KERNEL ||
      F.hasFnAttribute("cgp1_kernel")) {
    auto It = MBB.begin();
    const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();
    DebugLoc DL = (It != MBB.end()) ? It->getDebugLoc() : DebugLoc();
    BuildMI(MBB, It, DL, TII.get(CGP1::LOADI), CGP1::R15).addImm(-1);  // 0xFFFFFFFF
  }
  // ... rest of prologue
}
```

#### Divergent Branch Handling

**File:** `llvm/lib/Target/CGP1/CGP1ISelLowering.cpp`

**Key Function:** `CGP1TargetLowering::LowerBR_CC()`

**Implementation:**
```cpp
SDValue CGP1TargetLowering::LowerBR_CC(SDValue Op, SelectionDAG &DAG) const {
  // ... compute condition mask ...
  
  // For SIMT (kernel functions), automatically manage EXEC mask
  if (IsKernel) {
    // Get current EXEC mask (R15)
    SDValue Exec = DAG.getCopyFromReg(Chain, DL, CGP1::R15, MVT::i32);
    Chain = Exec.getValue(1);
    
    // Push EXEC mask onto stack (for reconvergence)
    SDValue ExecPush = DAG.getNode(CGP1ISD::ExecPush, DL, MVT::Other, Chain, Exec);
    Chain = ExecPush;
    
    // Compute EXEC mask for taken path: EXEC & condition_mask
    SDValue TakenExec = DAG.getNode(ISD::AND, DL, MVT::i32, Exec, FullConditionMask);
    
    // Compute EXEC mask for fall-through path: EXEC & NOT condition_mask
    SDValue NotConditionMask = DAG.getNode(ISD::XOR, DL, MVT::i32, FullConditionMask, AllOnes);
    SDValue FallThroughExec = DAG.getNode(ISD::AND, DL, MVT::i32, Exec, NotConditionMask);
    
    // Create SIMT branch node with both EXEC masks and both destinations
    return DAG.getNode(CGP1ISD::SIMTBranch, DL, MVT::Other, Chain,
                       ConditionMask, TakenExec, FallThroughExec, Dest, FallThroughDest);
  }
  // ... non-kernel branch handling ...
}
```

**Custom DAG Nodes:**
```cpp
// In CGP1ISelLowering.h
namespace CGP1ISD {
  enum NodeType {
    // ... other nodes ...
    ExecPush,        // Push EXEC mask onto stack
    ExecPop,          // Pop EXEC mask from stack
    ExecReconverge,   // Reconvergence point
    SIMTBranch,       // SIMT branch with EXEC mask management
  };
}
```

#### Reconvergence Pass

**File:** `llvm/lib/Target/CGP1/CGP1SIMTReconvergence.cpp` (NEW FILE)

**Purpose:** Automatically insert EXEC mask restoration at reconvergence points (basic blocks with multiple predecessors).

**Key Functions:**
```cpp
void CGP1SIMTReconvergence::handleReconvergencePoint(MachineFunction &MF,
                                                     MachineBasicBlock *MBB) {
  if (MBB->pred_size() < 2)
    return; // Not a reconvergence point
  
  // Pop EXEC mask from stack and restore to R15
  BuildMI(*MBB, MBBI, DL, TII->get(CGP1::POP), CGP1::R15);
}

void CGP1SIMTReconvergence::handleFallThroughPath(MachineFunction &MF,
                                                   MachineBasicBlock *MBB) {
  // Update EXEC mask for fall-through path from SIMT branch
  // ...
}
```

**Registration:**
```cpp
// In CGP1TargetMachine.cpp
void CGP1PassConfig::addPreEmitPass() override {
  // Add SIMT reconvergence pass to handle automatic EXEC mask management
  addPass(createCGP1SIMTReconvergencePass());
}
```

#### Stack Operations for EXEC Mask

**Files Modified:**
- `llvm/lib/Target/CGP1/CGP1InstrInfo.td` - PUSH/POP instructions

**Instructions:**
```tablegen
// Push to stack (store + decrement SP)
def PUSH : CGP1InstStore<0b10100, (outs), (ins GPR:$rb),
                         "push\t$rb", [(CGP1execpush GPR:$rb)]>;

// Pop from stack (increment SP + load)
def POP : CGP1InstLoad<0b10011, (outs GPR:$rc), (ins),
                       "pop\t$rc", [(set GPR:$rc, (CGP1execpop))]>;
```

---

## 3. Memory Spaces: Global, Shared, and Local

### What It Is

GPUs have multiple memory spaces with different characteristics:

- **Global Memory**: Large, slow, accessible by all threads (like RAM)
- **Shared Memory**: Fast on-chip memory shared within a block (like L1 cache)
- **Local Memory**: Per-thread private memory (usually in global memory)

### Why It Matters

- **Performance**: Shared memory is 10-100x faster than global memory
- **Cooperation**: Threads in a block can share data via shared memory
- **Correctness**: Different address spaces prevent accidental data races

### Implementation in CGP1

#### Address Space Mapping

**Files Modified:**
- `clang/lib/Basic/Targets/CGP1.h` - Data layout and address space definitions

**Data Layout:**
```cpp
// In CGP1.h
resetDataLayout("E-m:m-p:32:32-p1:32:32-p3:32:32-p4:32:32"
                "-i8:8:32-i16:16:32-i64:64-n32-S64");
```

**Address Spaces:**
- `p:32:32` - Default/private pointers (address space 0)
- `p1:32:32` - Global memory pointers (address space 1)
- `p3:32:32` - Shared/local memory pointers (address space 3)
- `p4:32:32` - Constant memory pointers (address space 4)

#### Shared Memory Intrinsics

**Files Modified:**
- `llvm/include/llvm/IR/IntrinsicsCGP1.td` - Intrinsic definitions
- `clang/include/clang/Basic/BuiltinsCGP1.def` - Builtin definitions
- `llvm/lib/Target/CGP1/CGP1ISelLowering.cpp` - Intrinsic lowering
- `llvm/lib/Target/CGP1/CGP1ISelDAGToDAG.cpp` - Instruction selection

**Intrinsics:**
```tablegen
// In IntrinsicsCGP1.td
def int_cgp1_shared_load : Intrinsic<[llvm_i32_ty], [llvm_i32_ty],
                                     [IntrReadMem, IntrWillReturn]>;
def int_cgp1_shared_store : Intrinsic<[], [llvm_i32_ty, llvm_i32_ty],
                                      [IntrWriteMem, IntrWillReturn]>;
```

**Lowering:**
```cpp
// In CGP1ISelLowering.cpp
case Intrinsic::cgp1_shared_load: {
  SDValue Addr = Op.getOperand(2);
  return DAG.getNode(CGP1ISD::SharedLoad, DL, DAG.getVTList(VT, MVT::Other),
                     Addr, Chain);
}
case Intrinsic::cgp1_shared_store: {
  SDValue Addr = Op.getOperand(2);
  SDValue Val = Op.getOperand(3);
  return DAG.getNode(CGP1ISD::SharedStore, DL, MVT::Other, Addr, Val, Chain);
}
```

**Instruction Selection:**
```cpp
// In CGP1ISelDAGToDAG.cpp
case ISD::LOAD: {
  auto *LD = cast<LoadSDNode>(Node);
  if (LD->getAddressSpace() == 3) {  // Address space 3 = shared memory
    SDLoc DL(Node);
    SDValue Chain = LD->getChain();
    SDValue Addr = LD->getBasePtr();
    ReplaceNode(Node,
                CurDAG->getMachineNode(CGP1::SHARED_LOAD, DL,
                                       LD->getValueType(0), MVT::Other,
                                       Chain, Addr));
    return;
  }
  break;
}
```

#### Shared Memory Instructions

**Files Modified:**
- `llvm/lib/Target/CGP1/CGP1InstrInfo.td` - Instruction definitions
- `llvm/lib/Target/CGP1/MCTargetDesc/CGP1MCCodeEmitter.cpp` - Binary encoding

**Instructions:**
```tablegen
// Opcode 0b11101
def SHARED_LOAD : CGP1Inst<(outs GPR:$rd), (ins GPR:$addr),
                           "shared_load\t$rd, $addr",
                           [(set GPR:$rd, (int_cgp1_shared_load GPR:$addr))]>;

def SHARED_STORE : CGP1Inst<(outs), (ins GPR:$addr, GPR:$val),
                            "shared_store\t$addr, $val",
                            [(int_cgp1_shared_store GPR:$addr, GPR:$val)]>;
```

**Binary Encoding:**
```cpp
// In CGP1MCCodeEmitter.cpp
case CGP1::SHARED_LOAD:
  Binary = encodeInstr(0b11101, COND_AL, 0, 0b0000, getReg(1), 0, getReg(0));
  break;
case CGP1::SHARED_STORE:
  Binary = encodeInstr(0b11101, COND_AL, 0, 0b0001, getReg(1), getReg(0), 0);
  break;
```

---

## 4. Synchronization: Barriers

### What It Is

A **barrier** synchronizes all threads in a block. When a thread reaches a barrier, it waits until all threads in the block reach the same barrier before continuing.

### Why It Matters

- **Data consistency**: Ensures shared memory writes are visible to all threads
- **Coordination**: Allows threads to work in phases (e.g., load data, compute, store results)
- **Correctness**: Prevents race conditions in parallel algorithms

### Implementation in CGP1

#### Barrier Intrinsic

**Files Modified:**
- `llvm/include/llvm/IR/IntrinsicsCGP1.td` - Intrinsic definition
- `clang/include/clang/Basic/BuiltinsCGP1.def` - Builtin definition
- `llvm/lib/Target/CGP1/CGP1ISelLowering.cpp` - Intrinsic lowering

**Intrinsic:**
```tablegen
// In IntrinsicsCGP1.td
def int_cgp1_barrier : Intrinsic<[], [], [IntrConvergent, IntrHasSideEffects]>;
```

**Builtin:**
```cpp
// In BuiltinsCGP1.def
BUILTIN(__builtin_cgp1_barrier, "v", "n")
```

**Lowering:**
```cpp
// In CGP1ISelLowering.cpp
case Intrinsic::cgp1_barrier:
  // Emit BARRIER instruction for thread synchronization
  return DAG.getNode(CGP1ISD::Barrier, DL, MVT::Other, Chain);
```

#### Barrier Instruction

**Files Modified:**
- `llvm/lib/Target/CGP1/CGP1InstrInfo.td` - Instruction definition
- `llvm/lib/Target/CGP1/MCTargetDesc/CGP1MCCodeEmitter.cpp` - Binary encoding

**Instruction:**
```tablegen
// Opcode 0b11011
let hasSideEffects = 1, isBarrier = 1 in
def BARRIER : CGP1Inst<(outs), (ins), "barrier", [(CGP1barrier)]> {
  let opcode = 0b11011;
}
```

**Binary Encoding:**
```cpp
// In CGP1MCCodeEmitter.cpp
case CGP1::BARRIER:
  Binary = encodeInstr(0b11011, COND_AL, 0, 0, 0, 0, 0);
  break;
```

**Properties:**
- `hasSideEffects = 1`: Barrier has side effects (synchronization)
- `isBarrier = 1`: Marks this as a barrier instruction (prevents reordering)
- `IntrConvergent`: LLVM intrinsic property indicating threads must reconverge

---

## 5. Atomic Operations

### What It Is

**Atomic operations** perform read-modify-write operations on memory that are guaranteed to be indivisible. Multiple threads can safely update the same memory location without explicit locking.

### Why It Matters

- **Lock-free algorithms**: Enables efficient parallel data structures
- **Reduction operations**: Sum, max, min across threads
- **Counters**: Thread-safe increment/decrement
- **Performance**: Faster than mutexes for fine-grained synchronization

### Implementation in CGP1

#### Atomic Intrinsics

**Files Modified:**
- `llvm/include/llvm/IR/IntrinsicsCGP1.td` - Intrinsic definitions
- `clang/include/clang/Basic/BuiltinsCGP1.def` - Builtin definitions
- `llvm/lib/Target/CGP1/CGP1ISelLowering.cpp` - Intrinsic lowering

**Intrinsics:**
```tablegen
// In IntrinsicsCGP1.td
def int_cgp1_atomic_add : Intrinsic<[llvm_i32_ty], [llvm_ptr_ty, llvm_i32_ty],
                                    [IntrArgMemOnly, IntrWillReturn]>;
def int_cgp1_atomic_cas : Intrinsic<[llvm_i32_ty], [llvm_ptr_ty, llvm_i32_ty, llvm_i32_ty],
                                    [IntrArgMemOnly, IntrWillReturn]>;
def int_cgp1_atomic_max : Intrinsic<[llvm_i32_ty], [llvm_ptr_ty, llvm_i32_ty],
                                    [IntrArgMemOnly, IntrWillReturn]>;
def int_cgp1_atomic_min : Intrinsic<[llvm_i32_ty], [llvm_ptr_ty, llvm_i32_ty],
                                    [IntrArgMemOnly, IntrWillReturn]>;
def int_cgp1_atomic_exch : Intrinsic<[llvm_i32_ty], [llvm_ptr_ty, llvm_i32_ty],
                                     [IntrArgMemOnly, IntrWillReturn]>;
```

**Lowering:**
```cpp
// In CGP1ISelLowering.cpp
case Intrinsic::cgp1_atomic_add: {
  SDValue Addr = Op.getOperand(2);
  SDValue Val = Op.getOperand(3);
  return DAG.getNode(CGP1ISD::AtomicAdd, DL, DAG.getVTList(VT, MVT::Other),
                     Addr, Val, Chain);
}
case Intrinsic::cgp1_atomic_max: {
  SDValue Addr = Op.getOperand(2);
  SDValue Val = Op.getOperand(3);
  return DAG.getNode(CGP1ISD::AtomicMax, DL, DAG.getVTList(VT, MVT::Other),
                     Addr, Val, Chain);
}
// ... (similar for other atomics)
```

#### Atomic Instructions

**Files Modified:**
- `llvm/lib/Target/CGP1/CGP1InstrInfo.td` - Instruction definitions
- `llvm/lib/Target/CGP1/MCTargetDesc/CGP1MCCodeEmitter.cpp` - Binary encoding

**Instructions:**
```tablegen
// Opcode 0b11110 with sel field distinguishing operation type
let hasSideEffects = 1, mayLoad = 1, mayStore = 1 in {
  def ATOMIC_ADD : CGP1Inst<(outs GPR:$rd), (ins GPR:$addr, GPR:$val),
                            "atomic_add\t$rd, $addr, $val",
                            [(set GPR:$rd, (CGP1atomic_add GPR:$addr, GPR:$val))]>;

  def ATOMIC_MAX : CGP1Inst<(outs GPR:$rd), (ins GPR:$addr, GPR:$val),
                            "atomic_max\t$rd, $addr, $val",
                            [(set GPR:$rd, (CGP1atomic_max GPR:$addr, GPR:$val))]>;

  def ATOMIC_MIN : CGP1Inst<(outs GPR:$rd), (ins GPR:$addr, GPR:$val),
                            "atomic_min\t$rd, $addr, $val",
                            [(set GPR:$rd, (CGP1atomic_min GPR:$addr, GPR:$val))]>;

  def ATOMIC_EXCH : CGP1Inst<(outs GPR:$rd), (ins GPR:$addr, GPR:$val),
                             "atomic_exch\t$rd, $addr, $val",
                             [(set GPR:$rd, (CGP1atomic_exch GPR:$addr, GPR:$val))]>;

  def ATOMIC_CAS : CGP1Inst<(outs GPR:$rd), (ins GPR:$addr, GPR:$cmp, GPR:$val),
                            "atomic_cas\t$rd, $addr, $cmp, $val",
                            [(set GPR:$rd, (CGP1atomic_cas GPR:$addr, GPR:$cmp, GPR:$val))]>;
}
```

**Binary Encoding:**
```cpp
// In CGP1MCCodeEmitter.cpp
case CGP1::ATOMIC_ADD:
  Binary = encodeInstr(0b11110, COND_AL, 0, 0b0000, getReg(1), getReg(0), 0);
  break;
case CGP1::ATOMIC_MAX:
  Binary = encodeInstr(0b11110, COND_AL, 0, 0b0010, getReg(1), getReg(0), 0);
  break;
// ... (similar for other atomics)
```

---

## 6. Warp-Level Primitives

### What It Is

A **warp** is a group of threads (typically 32) that execute in lockstep. Warp-level primitives allow threads within a warp to:

- **Vote**: Check if all/any threads meet a condition
- **Shuffle**: Exchange data between threads
- **Ballot**: Get a bitmask of threads meeting a condition

### Why It Matters

- **Efficiency**: Warp operations are hardware-accelerated
- **Communication**: Threads can share data without using shared memory
- **Optimization**: Enables efficient reductions and scans

### Implementation in CGP1

#### Warp Intrinsics

**Files Modified:**
- `llvm/include/llvm/IR/IntrinsicsCGP1.td` - Intrinsic definitions
- `clang/include/clang/Basic/BuiltinsCGP1.def` - Builtin definitions
- `llvm/lib/Target/CGP1/CGP1ISelLowering.cpp` - Intrinsic lowering

**Intrinsics:**
```tablegen
// In IntrinsicsCGP1.td
def int_cgp1_warp_size : Intrinsic<[llvm_i32_ty], [], [...]>;  // Returns 32
def int_cgp1_lane_id : Intrinsic<[llvm_i32_ty], [], [...]>;   // 0-31

def int_cgp1_warp_all : Intrinsic<[llvm_i32_ty], [llvm_i32_ty],
                                   [IntrConvergent, IntrNoMem]>;
def int_cgp1_warp_any : Intrinsic<[llvm_i32_ty], [llvm_i32_ty],
                                  [IntrConvergent, IntrNoMem]>;
def int_cgp1_warp_ballot : Intrinsic<[llvm_i32_ty], [llvm_i32_ty],
                                     [IntrConvergent, IntrNoMem]>;
def int_cgp1_warp_shuffle : Intrinsic<[llvm_i32_ty], [llvm_i32_ty, llvm_i32_ty],
                                      [IntrConvergent, IntrNoMem]>;
```

**Lowering:**
```cpp
// In CGP1ISelLowering.cpp
case Intrinsic::cgp1_warp_size:
  return DAG.getNode(CGP1ISD::WarpSize, DL, VT);
case Intrinsic::cgp1_lane_id:
  return DAG.getNode(CGP1ISD::LaneId, DL, VT);
case Intrinsic::cgp1_warp_all: {
  SDValue Pred = Op.getOperand(1);
  return DAG.getNode(CGP1ISD::WarpAll, DL, VT, Pred);
}
case Intrinsic::cgp1_warp_shuffle: {
  SDValue Val = Op.getOperand(1);
  SDValue Lane = Op.getOperand(2);
  return DAG.getNode(CGP1ISD::WarpShuffle, DL, VT, Val, Lane);
}
// ... (similar for warp_any, warp_ballot)
```

#### Warp Instructions

**Files Modified:**
- `llvm/lib/Target/CGP1/CGP1InstrInfo.td` - Instruction definitions
- `llvm/lib/Target/CGP1/MCTargetDesc/CGP1MCCodeEmitter.cpp` - Binary encoding

**Instructions:**
```tablegen
// Opcode 0b11111 with sel field distinguishing operation
def WARP_SIZE : CGP1Inst<(outs GPR:$rd), (ins), "warp_size\t$rd",
                         [(set GPR:$rd, (CGP1warp_size))]>;

def LANE_ID : CGP1Inst<(outs GPR:$rd), (ins), "lane_id\t$rd",
                       [(set GPR:$rd, (CGP1lane_id))]>;

def WARP_ALL : CGP1Inst<(outs GPR:$rd), (ins GPR:$pred),
                        "warp_all\t$rd, $pred",
                        [(set GPR:$rd, (CGP1warp_all GPR:$pred))]>;

def WARP_SHUFFLE : CGP1Inst<(outs GPR:$rd), (ins GPR:$val, GPR:$lane),
                            "warp_shuffle\t$rd, $val, $lane",
                            [(set GPR:$rd, (CGP1warp_shuffle GPR:$val, GPR:$lane))]>;
```

**Binary Encoding:**
```cpp
// In CGP1MCCodeEmitter.cpp
case CGP1::WARP_SIZE:
  Binary = encodeInstr(0b11111, COND_AL, 0, 0b0000, 0, 0, getReg(0));
  break;
case CGP1::LANE_ID:
  Binary = encodeInstr(0b11111, COND_AL, 0, 0b0001, 0, 0, getReg(0));
  break;
// ... (similar for other warp operations)
```

---

## 7. Graphics Operations

### What It Is

Graphics operations provide direct access to the GPU's framebuffer and graphics pipeline, enabling:

- **Pixel operations**: Write/read pixels
- **Primitive drawing**: Lines, triangles, rectangles, circles
- **Rasterization**: Hardware-accelerated shape rendering

### Why It Matters

- **Performance**: Hardware-accelerated graphics operations
- **Simplicity**: Direct API without needing OpenGL/Vulkan
- **Embedded GPUs**: Essential for display controllers and embedded graphics

### Implementation in CGP1

#### Graphics Intrinsics

**Files Modified:**
- `llvm/include/llvm/IR/IntrinsicsCGP1.td` - Intrinsic definitions
- `clang/include/clang/Basic/BuiltinsCGP1.def` - Builtin definitions
- `llvm/lib/Target/CGP1/CGP1ISelLowering.cpp` - Intrinsic lowering

**Basic Graphics Intrinsics:**
```tablegen
// In IntrinsicsCGP1.td
def int_cgp1_write_pixel : Intrinsic<[], [llvm_i32_ty, llvm_i32_ty, llvm_i32_ty],
                                     [IntrWriteMem, IntrWillReturn]>;
def int_cgp1_clear_screen : Intrinsic<[], [llvm_i32_ty],
                                      [IntrWriteMem, IntrWillReturn]>;
def int_cgp1_draw_line : Intrinsic<[], [llvm_i32_ty, llvm_i32_ty, llvm_i32_ty,
                                        llvm_i32_ty, llvm_i32_ty],
                                   [IntrWriteMem, IntrWillReturn]>;
def int_cgp1_draw_triangle : Intrinsic<[], [llvm_i32_ty, llvm_i32_ty, llvm_i32_ty,
                                            llvm_i32_ty, llvm_i32_ty, llvm_i32_ty,
                                            llvm_i32_ty],
                                       [IntrWriteMem, IntrWillReturn]>;
```

**Lowering for write_pixel:**
```cpp
// In CGP1ISelLowering.cpp
case Intrinsic::cgp1_write_pixel: {
  SDValue X = Op.getOperand(2);
  SDValue Y = Op.getOperand(3);
  SDValue Color = Op.getOperand(4);

  // Calculate pixel offset: (y * FRAMEBUFFER_WIDTH + x) * 4
  SDValue Width = DAG.getConstant(FRAMEBUFFER_WIDTH, DL, MVT::i32);
  SDValue YxWidth = DAG.getNode(ISD::MUL, DL, MVT::i32, Y, Width);
  SDValue Offset = DAG.getNode(ISD::ADD, DL, MVT::i32, YxWidth, X);
  SDValue ByteOffset = DAG.getNode(ISD::SHL, DL, MVT::i32, Offset,
                                   DAG.getConstant(2, DL, MVT::i32)); // * 4

  // Calculate absolute address: FRAMEBUFFER_BASE + byte_offset
  SDValue BaseAddr = DAG.getConstant(FRAMEBUFFER_BASE, DL, MVT::i32);
  SDValue Addr = DAG.getNode(ISD::ADD, DL, MVT::i32, BaseAddr, ByteOffset);

  // Store color to framebuffer
  return DAG.getStore(Chain, DL, Color, Addr, MachinePointerInfo());
}
```

**Lowering for draw_line/draw_triangle:**
```cpp
case Intrinsic::cgp1_draw_line: {
  // Copy args to registers with glue to prevent reordering
  SDValue Glue;
  Chain = DAG.getCopyToReg(Chain, DL, CGP1::R1, X0, Glue);
  Glue = Chain.getValue(1);
  Chain = DAG.getCopyToReg(Chain, DL, CGP1::R2, Y0, Glue);
  // ... (copy all args to R1-R5)
  return DAG.getNode(CGP1ISD::DrawLine, DL, MVT::Other, Chain, Glue);
}
```

#### Graphics Instructions

**Files Modified:**
- `llvm/lib/Target/CGP1/CGP1InstrInfo.td` - Instruction definitions
- `llvm/lib/Target/CGP1/MCTargetDesc/CGP1MCCodeEmitter.cpp` - Binary encoding

**Instructions:**
```tablegen
// Opcode 0b11100 with sel field distinguishing operation
let hasSideEffects = 1, mayStore = 1 in {
  def WRITE_PIXEL : CGP1Inst<(outs), (ins GPR:$x, GPR:$y, GPR:$color),
                             "write_pixel\t$x, $y, $color",
                             [(int_cgp1_write_pixel GPR:$x, GPR:$y, GPR:$color)]>;
}

let hasSideEffects = 1 in {
  def CLEAR_SCREEN : CGP1Inst<(outs), (ins),
                              "clear_screen",
                              [(CGP1clearscreen)]>;
  
  def DRAW_LINE : CGP1Inst<(outs), (ins),
                           "draw_line",
                           [(CGP1drawline)]>;
  
  def DRAW_TRIANGLE : CGP1Inst<(outs), (ins),
                               "draw_triangle",
                               [(CGP1drawtriangle)]>;
}
```

**Binary Encoding:**
```cpp
// In CGP1MCCodeEmitter.cpp
case CGP1::WRITE_PIXEL:
  Binary = encodeInstr(0b11100, COND_AL, 0, 0b0000, getReg(0), getReg(1), getReg(2));
  break;
case CGP1::CLEAR_SCREEN:
  Binary = encodeInstr(0b11100, COND_AL, 0, 0b0001, getReg(0), 0, 0);
  break;
```

---

## 8. Kernel vs Device Functions

### What It Is

**Kernels** are entry points launched by the host CPU. They:
- Cannot return values
- Terminate with `STOP_CORE` instruction
- Receive thread/block IDs via special registers
- Initialize EXEC mask to all-ones

**Device functions** are regular functions called from kernels:
- Can return values
- Use normal calling convention
- Terminate with `RET` instruction

### Why It Matters

- **Separation**: Kernels are the GPU entry points, device functions are helpers
- **Optimization**: Different calling conventions and optimizations
- **Correctness**: Kernels must handle SIMT execution model

### Implementation in CGP1

#### Kernel Detection

**Files Modified:**
- `llvm/lib/Target/CGP1/CGP1ISelLowering.cpp` - Kernel detection function

**Function:**
```cpp
// In CGP1ISelLowering.cpp
bool CGP1TargetLowering::isKernelFunction(const Function &F) const {
  if (F.getCallingConv() == CallingConv::SPIR_KERNEL)
    return true;
  if (F.hasFnAttribute("cgp1_kernel"))
    return true;
  return false;
}
```

#### Kernel Argument Setup

**Files Modified:**
- `llvm/lib/Target/CGP1/CGP1ISelLowering.cpp` - `LowerFormalArguments()`

**Code:**
```cpp
SDValue CGP1TargetLowering::LowerFormalArguments(...) const {
  MachineFunction &MF = DAG.getMachineFunction();
  MachineRegisterInfo &RegInfo = MF.getRegInfo();

  bool IsKernel = isKernelFunction(MF.getFunction());
  
  if (IsKernel) {
    // For kernels, we use R14 (LR) to pass the thread ID (threadIdx.x).
    // R11 for block_id_x, R10 for block_dim_x, and R9 for grid_dim_x.
    
    // Thread ID
    Register VRegThreadID = RegInfo.createVirtualRegister(&CGP1::GPRRegClass);
    RegInfo.addLiveIn(CGP1::R14, VRegThreadID);
    
    // Block ID
    Register VRegBlockID = RegInfo.createVirtualRegister(&CGP1::GPRRegClass);
    RegInfo.addLiveIn(CGP1::R11, VRegBlockID);
    
    // Block Dim
    Register VRegBlockDim = RegInfo.createVirtualRegister(&CGP1::GPRRegClass);
    RegInfo.addLiveIn(CGP1::R10, VRegBlockDim);
    
    // Grid Dim
    Register VRegGridDim = RegInfo.createVirtualRegister(&CGP1::GPRRegClass);
    RegInfo.addLiveIn(CGP1::R9, VRegGridDim);
  }
  // ... handle regular arguments ...
}
```

#### Kernel Return (EndKernel)

**Files Modified:**
- `llvm/lib/Target/CGP1/CGP1ISelLowering.cpp` - `LowerReturn()`

**Code:**
```cpp
SDValue CGP1TargetLowering::LowerReturn(...) const {
  MachineFunction &MF = DAG.getMachineFunction();
  bool IsKernel = isKernelFunction(MF.getFunction());
  
  if (IsKernel && Outs.size() > 0) {
    report_fatal_error("Kernel functions cannot return values", false);
  }
  
  if (Outs.size() == 0) {
    // Kernels terminate with EndKernel (STOP_CORE), device functions use Ret
    if (IsKernel)
      return DAG.getNode(CGP1ISD::EndKernel, dl, MVT::Other, Chain);
    return DAG.getNode(CGP1ISD::Ret, dl, MVT::Other, Chain);
  }
  // ... handle return values for device functions ...
}
```

#### STOP_CORE Instruction

**Files Modified:**
- `llvm/lib/Target/CGP1/CGP1InstrInfo.td` - Instruction definition
- `llvm/lib/Target/CGP1/MCTargetDesc/CGP1MCCodeEmitter.cpp` - Binary encoding

**Instruction:**
```tablegen
// Opcode 0b11000
let hasSideEffects = 1, isTerminator = 1, isReturn = 1, isBarrier = 1 in
def STOP_CORE : CGP1Inst<(outs), (ins), "stop_core", [(CGP1endkernel)]> {
  let opcode = 0b11000;
}
```

**Binary Encoding:**
```cpp
// In CGP1MCCodeEmitter.cpp
case CGP1::STOP_CORE:
  Binary = encodeInstr(0b11000, COND_AL, 0, 0, 0, 0, 0);
  break;
```

---

## 9. Fast Math Intrinsics

### What It Is

Fast math intrinsics provide hardware-accelerated or optimized implementations of common mathematical functions:

- **Reciprocal**: 1/x
- **Reciprocal square root**: 1/√x
- **Trigonometric**: sin, cos
- **Exponential/Logarithmic**: exp, log

### Why It Matters

- **Performance**: Hardware acceleration is much faster than software implementations
- **Precision trade-offs**: Fast versions may have lower precision but acceptable for graphics
- **Common operations**: These functions are used frequently in GPU code

### Implementation in CGP1

#### Fast Math Intrinsics

**Files Modified:**
- `llvm/include/llvm/IR/IntrinsicsCGP1.td` - Intrinsic definitions
- `clang/include/clang/Basic/BuiltinsCGP1.def` - Builtin definitions
- `llvm/lib/Target/CGP1/CGP1ISelLowering.cpp` - Intrinsic lowering (if custom)

**Intrinsics:**
```tablegen
// In IntrinsicsCGP1.td
def int_cgp1_rcp : Intrinsic<[llvm_float_ty], [llvm_float_ty],
                             [IntrNoMem, IntrSpeculatable]>;
def int_cgp1_rsqrt : Intrinsic<[llvm_float_ty], [llvm_float_ty],
                               [IntrNoMem, IntrSpeculatable]>;
def int_cgp1_sin : Intrinsic<[llvm_float_ty], [llvm_float_ty],
                             [IntrNoMem, IntrSpeculatable]>;
def int_cgp1_cos : Intrinsic<[llvm_float_ty], [llvm_float_ty],
                             [IntrNoMem, IntrSpeculatable]>;
def int_cgp1_exp : Intrinsic<[llvm_float_ty], [llvm_float_ty],
                             [IntrNoMem, IntrSpeculatable]>;
def int_cgp1_log : Intrinsic<[llvm_float_ty], [llvm_float_ty],
                             [IntrNoMem, IntrSpeculatable]>;
```

#### Fast Math Instructions

**Files Modified:**
- `llvm/lib/Target/CGP1/CGP1InstrInfo.td` - Instruction definitions
- `llvm/lib/Target/CGP1/MCTargetDesc/CGP1MCCodeEmitter.cpp` - Binary encoding

**Instructions:**
```tablegen
// Reuse FP opcodes with different sel values
def FRCP : CGP1InstUnary<0b00110, 0b0001,  // fdiv opcode, sel=1
                         (outs GPR:$rc), (ins GPR:$ra),
                         "frcp\t$rc, $ra",
                         [(set GPR:$rc, (int_cgp1_rcp GPR:$ra))]>;

def FRSQRT : CGP1InstUnary<0b00110, 0b0010,  // fdiv opcode, sel=2
                           (outs GPR:$rc), (ins GPR:$ra),
                           "frsqrt\t$rc, $ra",
                           [(set GPR:$rc, (int_cgp1_rsqrt GPR:$ra))]>;

def FSIN : CGP1InstUnary<0b01000, 0b0100,  // trunc/round opcode, sel=4
                         (outs GPR:$rc), (ins GPR:$ra),
                         "fsin\t$rc, $ra",
                         [(set GPR:$rc, (int_cgp1_sin GPR:$ra))]>;

def FCOS : CGP1InstUnary<0b01000, 0b0101,  // trunc/round opcode, sel=5
                         (outs GPR:$rc), (ins GPR:$ra),
                         "fcos\t$rc, $ra",
                         [(set GPR:$rc, (int_cgp1_cos GPR:$ra))]>;
```

**Binary Encoding:**
```cpp
// In CGP1MCCodeEmitter.cpp
case CGP1::FRCP:
  Binary = encodeInstr(0b00110, COND_AL, 0, 0b0001, getReg(1), 0, getReg(0));
  break;
case CGP1::FRSQRT:
  Binary = encodeInstr(0b00110, COND_AL, 0, 0b0010, getReg(1), 0, getReg(0));
  break;
// ... (similar for sin, cos, exp, log)
```

---

## Summary: Files Changed by Feature

### Core Infrastructure
- `llvm/lib/Target/CGP1/CGP1TargetMachine.cpp` - Pass registration
- `llvm/lib/Target/CGP1/CGP1ISelLowering.h` - Custom DAG nodes
- `llvm/lib/Target/CGP1/CGP1ISelLowering.cpp` - Intrinsic lowering, kernel detection
- `llvm/lib/Target/CGP1/CGP1FrameLowering.cpp` - EXEC mask initialization
- `llvm/lib/Target/CGP1/CGP1SIMTReconvergence.cpp` - **NEW FILE** - Reconvergence pass

### Frontend Integration
- `llvm/include/llvm/IR/IntrinsicsCGP1.td` - **NEW FILE** - LLVM intrinsics
- `clang/include/clang/Basic/BuiltinsCGP1.def` - **NEW FILE** - Clang builtins
- `clang/lib/CodeGen/CGBuiltin.cpp` - Builtin code generation
- `clang/lib/Basic/Targets/CGP1.h` - Target info, address spaces
- `clang/lib/Basic/Targets/CGP1.cpp` - Target implementation

### Instruction Set
- `llvm/lib/Target/CGP1/CGP1InstrInfo.td` - Instruction definitions
- `llvm/lib/Target/CGP1/CGP1ISelDAGToDAG.cpp` - Instruction selection
- `llvm/lib/Target/CGP1/MCTargetDesc/CGP1MCCodeEmitter.cpp` - Binary encoding

### Register and Calling Convention
- `llvm/lib/Target/CGP1/CGP1RegisterInfo.cpp` - Register file
- `llvm/lib/Target/CGP1/CGP1CallingConv.td` - Calling convention

---

## References

- **LLVM Target-Independent Code Generator**: https://llvm.org/docs/CodeGenerator.html
- **Writing an LLVM Backend**: https://llvm.org/docs/WritingAnLLVMBackend.html
- **GPU Computing**: CUDA/OpenCL programming models
- **SIMT Execution**: AMD GCN Architecture, NVIDIA CUDA Cores

---

*Last Updated: [Current Date]*
*CGP1 GPU Backend Version: 1.0*

