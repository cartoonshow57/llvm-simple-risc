# CGP1: Compile to flat binary (.bin)

This documents the current end-to-end flow for turning CGP1 C/assembly into a flat binary suitable for loading at address `0x0` (no headers), using the custom CGP1 toolchain built under `llvm-project/build`.

## Prereqs
- CGP1 backend built: `llvm-project/build/bin/clang` and `llvm-project/build/bin/llvm-objcopy`
- Input source: `.c`, `.s`, or `.o`
- Working directory: repository root (`/home/car/llvm-dev/llvm-cgp1`)

## Quick start
```bash
# From repo root
./scripts/cgp1-make-bin.sh tests/vertex_shader.c -o vertex_shader.bin
```
This produces a flat binary `vertex_shader.bin` at load address `0x0`.

## Script details
- Location: `scripts/cgp1-make-bin.sh`
- Accepts `.c`, `.s`, or `.o`
- Steps:
  1) Compile to object with `clang --target=cgp1-unknown-unknown -c`
  2) Strip to flat binary with `llvm-objcopy -O binary`
- Output defaults to `<input_basename>.bin` if `-o` is omitted

## Example commands
```bash
# Generate assembly
./llvm-project/build/bin/clang --target=cgp1-unknown-unknown -S tests/vertex_shader.c -o vertex_shader.s

# Generate object
./llvm-project/build/bin/clang --target=cgp1-unknown-unknown -c tests/vertex_shader.c -o vertex_shader.o

# Generate flat binary (direct)
./scripts/cgp1-make-bin.sh tests/vertex_shader.c -o vertex_shader.bin

# Generate flat binary from an existing object
./scripts/cgp1-make-bin.sh vertex_shader.o -o vertex_shader.bin
```

## Notes and limitations
- Output is a **flat** binary with no headers; load at address `0x0`.
- External symbols (e.g., `memset`) are not resolved; prefer self-contained code when targeting the GPU/emulator.
- If `GPU Assembler.pdf` specifies additional headers/metadata, we can extend the script to prepend them, but the current flow matches the minimal flat format.

