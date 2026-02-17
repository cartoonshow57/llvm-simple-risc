# CGP1 GPU Backend Documentation

## Table of Contents
1. [Execution Model](#execution-model)
2. [Memory Model](#memory-model)
3. [SIMT Execution](#simt-execution)
4. [Thread Hierarchy](#thread-hierarchy)
5. [Synchronization](#synchronization)
6. [Intrinsic Functions](#intrinsic-functions)
7. [Instruction Set](#instruction-set)
8. [Code Generation](#code-generation)

## Execution Model

### SIMD Execution
- **Implementation**: The CGP1 backend implements a SIMT (Single Instruction, Multiple Threads) execution model.
- **Key Components**:
  - `CGP1SIMTReconvergence` pass handles control flow divergence
  - EXEC mask register (R15) tracks active threads
  - Stack-based approach for managing execution masks

### Kernel Execution
- **Entry Point**: Kernel functions are marked with `__kernel` or `__attribute__((cgp1_kernel))`
- **Initialization**:
  ```cpp
  // In CGP1FrameLowering.cpp
  if (F.getCallingConv() == CallingConv::SPIR_KERNEL ||
      F.hasFnAttribute("cgp1_kernel")) {
      // Initialize EXEC mask to all-ones for kernels
      BuildMI(MBB, It, DL, TII.get(CGP1::LOADI), CGP1::R15).addImm(-1);
  }
  ```

## Memory Model

### Address Spaces
- **Global Memory**: Default address space (0)
- **Shared Memory**: Address space 3
- **Constant Memory**: Address space 4
- **Private Memory**: Per-thread private memory

### Memory Operations
- **Shared Memory Access**:
  ```cpp
  // In CGP1ISelDAGToDAG.cpp
  if (LD->getAddressSpace() == 3) {
      // Shared memory load
      ReplaceNode(Node, CurDAG->getMachineNode(CGP1::SHARED_LOAD, ...));
  }
  ```

## SIMT Execution

### Divergence Handling
- **Reconvergence Points**:
  - Detected at basic blocks with multiple predecessors
  - EXEC mask is restored from the stack

- **Branch Handling**:
  ```cpp
  // In CGP1ISelLowering.cpp
  if (IsKernel) {
      // Push current EXEC mask
      SDValue ExecPush = DAG.getNode(CGP1ISD::ExecPush, DL, MVT::Other, Chain, Exec);
      // Compute new EXEC masks for taken and fall-through paths
      SDValue TakenExec = DAG.getNode(ISD::AND, DL, MVT::i32, Exec, FullConditionMask);
      // ...
  }
  ```

## Thread Hierarchy

### Thread Identification
- **Thread ID**: Accessed through `cgp1_thread_id_x/y/z` intrinsics
- **Warp Operations**:
  - `WARP_SIZE`: Returns the number of threads in a warp
  - `LANE_ID`: Returns the lane ID within a warp
  - `WARP_ALL`: Warp-wide vote operation
  - `WARP_ANY`: Warp-wide any operation
  - `WARP_BALLOT`: Warp-wide ballot operation
  - `WARP_SHUFFLE`: Warp-wide shuffle operation

## Synchronization

### Barriers
- **Implementation**:
  ```cpp
  // In CGP1InstrInfo.td
  def CGP1barrier : SDNode<"CGP1ISD::Barrier", SDTNone, [SDNPHasChain, SDNPSideEffect]>;
  ```
- **Usage**: `__syncthreads()` equivalent for thread synchronization

## Intrinsic Functions

### EXEC Mask Management
- `cgp1_exec_get`: Get current EXEC mask
- `cgp1_exec_set`: Set EXEC mask
- `cgp1_exec_and`: Bitwise AND with EXEC mask
- `cgp1_exec_push`: Push EXEC mask to stack
- `cgp1_exec_pop`: Pop EXEC mask from stack

### Thread Operations
- `cgp1_thread_id_x/y/z`: Get thread ID in block
- `cgp1_warp_size`: Get warp size
- `cgp1_lane_id`: Get lane ID in warp

## Instruction Set

### SIMD Instructions
- `V_ADD_F32`: SIMD floating-point addition
- `V_MUL_F32`: SIMD floating-point multiplication
- `V_MAD_F32`: Fused multiply-add

### Memory Instructions
- `LOAD_GLOBAL`: Load from global memory
- `STORE_GLOBAL`: Store to global memory
- `LOAD_SHARED`: Load from shared memory
- `STORE_SHARED`: Store to shared memory

## Code Generation

### Instruction Selection
- Pattern matching in `CGP1DAGToDAGISel::Select()`
- Special handling for SIMT branches and memory operations

### Register Allocation
- Physical register usage:
  - `R15`: EXEC mask
  - `R14`: Thread ID (in kernels)
  - `R12`: Frame pointer
  - `R13`: Stack pointer

### Control Flow
- **Divergent Branches**:
  - Handled by `CGP1SIMTReconvergence` pass
  - EXEC mask is managed at branch points and reconvergence points

## Implementation Files

### Core Components
- `CGP1TargetMachine.cpp`: Target machine setup and pass configuration
- `CGP1ISelLowering.cpp`: Lowering of LLVM IR to CGP1 DAG nodes
- `CGP1InstrInfo.td`: Instruction definitions
- `CGP1SIMTReconvergence.cpp`: SIMT control flow handling
- `CGP1ISelDAGToDAG.cpp`: Instruction selection

### Support Files
- `CGP1RegisterInfo.td`: Register definitions
- `CGP1CallingConv.td`: Calling convention
- `CGP1Subtarget.h/cpp`: Target-specific properties

## Future Work
- Improve SIMT divergence handling
- Add support for more GPU-specific operations
- Optimize memory access patterns
- Enhance debugging support
- Add more comprehensive testing
