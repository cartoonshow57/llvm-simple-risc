# CGP1 GPU Compiler Backend

Custom LLVM backend that compiles C to GPU assembly/machine code for the CGP1 ISA.

## Building

```bash
cd llvm-project
mkdir build && cd build
cmake -G "Unix Makefiles" \
      -DCMAKE_BUILD_TYPE=Release \
      -DLLVM_ENABLE_PROJECTS="clang;lld" \
      -DLLVM_TARGETS_TO_BUILD="CGP1" \
      ..
make -j$(nproc)
```

## Compiling Code

Target triple: `cgp1-unknown-unknown`

```bash
# C -> Assembly
./build/bin/clang --target=cgp1-unknown-unknown -S input.c -o output.s

# C -> Object file
./build/bin/clang --target=cgp1-unknown-unknown -c input.c -o output.o

# C -> Flat binary (loads at 0x0, no headers)
./scripts/cgp1-make-bin.sh input.c -o output.bin
```

The binary script also accepts `.s` and `.o` files as input.

## Writing Kernels

Mark GPU entry points with `__attribute__((cgp1_kernel))`. They can't return values and terminate with `STOP_CORE` automatically. Regular (device) functions work normally.

```c
__attribute__((cgp1_kernel))
void my_kernel() {
    int tid = __builtin_cgp1_thread_id_x();
    int bid = __builtin_cgp1_block_id_x();
    int global_id = bid * __builtin_cgp1_block_dim_x() + tid;
}
```

## Builtins

**Thread/Block:** `__builtin_cgp1_thread_id_x/y/z()`, `__builtin_cgp1_block_id_x/y/z()`, `__builtin_cgp1_block_dim_x/y/z()`, `__builtin_cgp1_grid_dim_x/y/z()`

**Sync:** `__builtin_cgp1_barrier()`

**Shared Memory:** `__builtin_cgp1_shared_load(addr)`, `__builtin_cgp1_shared_store(addr, val)`

**Atomics:** `__builtin_cgp1_atomic_add`, `_max`, `_min`, `_exch`, `_cas` — all take `(int* addr, int val)`, CAS takes an extra `cmp` arg

**Warp:** `__builtin_cgp1_warp_size()`, `_lane_id()`, `_warp_all(pred)`, `_warp_any(pred)`, `_warp_ballot(pred)`, `_warp_shuffle(val, lane)`

**Graphics:** `__builtin_cgp1_write_pixel(x, y, color)`, `__builtin_cgp1_clear_screen(color)`

**Fast Math:** `__builtin_cgp1_rcp`, `_rsqrt`, `_sin`, `_cos`, `_exp`, `_log` — all take and return float

## Register Conventions

| Register | Role |
|----------|------|
| R0-R8 | General purpose |
| R9 | gridDim.x (in kernels) |
| R10 | blockDim.x (in kernels) |
| R11 | blockIdx.x (in kernels) |
| R12 | Frame pointer |
| R13 | Stack pointer |
| R14 | Link register / threadIdx.x (in kernels) |
| R15 | EXEC mask (in kernels, init to 0xFFFFFFFF) |

## Key Files

```
llvm/lib/Target/CGP1/
  CGP1TargetMachine.cpp       - Target setup, pass registration
  CGP1ISelLowering.cpp        - IR lowering (most of the interesting code is here)
  CGP1ISelDAGToDAG.cpp        - Instruction selection
  CGP1SIMTReconvergence.cpp   - SIMT divergence/reconvergence pass
  CGP1FrameLowering.cpp       - Stack frames, EXEC mask init
  CGP1InstrInfo.td            - Instruction definitions
  CGP1RegisterInfo.td         - Register definitions
  MCTargetDesc/               - Machine code emission, encoding

clang/lib/Basic/Targets/CGP1.h/cpp    - Clang target info
clang/include/clang/Basic/BuiltinsCGP1.def  - Builtin definitions
llvm/include/llvm/IR/IntrinsicsCGP1.td      - LLVM intrinsics
scripts/cgp1-make-bin.sh                     - Binary generation script
```

## Adding Stuff

**New instruction:** `CGP1InstrInfo.td` → `CGP1MCCodeEmitter.cpp`

**New intrinsic/builtin:** `IntrinsicsCGP1.td` → `BuiltinsCGP1.def` → `CGP1ISelLowering.cpp` → `CGP1ISelDAGToDAG.cpp`

**New pass:** Create file → register in `CGP1TargetMachine.cpp`

## More Docs

- `docs/CGP1_GPU_Features_Implementation.md` — detailed feature writeup
- `llvm/lib/Target/CGP1/CGP1_ISA.md` — ISA reference
- Test files in repo root (`gpu_test_working.c`, `test_*.c`, etc.)
