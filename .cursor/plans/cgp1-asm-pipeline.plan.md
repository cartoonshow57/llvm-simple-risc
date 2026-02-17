# Plan: CGP1 native .s → machine code

## Goal

Enable the CGP1 backend to assemble `.s` directly to machine code (ELF/flat) using LLVM’s MC layer—no external scripts needed.

## Steps

1) MC layer scaffolding

- Add MCAsmInfo, register info, subtarget info hookups if any gaps.
- Ensure MCTargetDesc exposes `createMCAsmInfo`, `createMCCodeEmitter`, `createMCAsmBackend`, `createMCObjectWriter`.

2) AsmBackend + object writer

- Implement `CGP1AsmBackend` to apply fixups/relocs and emit bytes.
- Choose output: start with ELF via `MCELFObjectTargetWriter`; optionally add flat writer if PDF requires headerless binaries.
- Define fixup kinds and relocation handling for immediates/branches.

3) Asm parser

- Implement `CGP1AsmParser` (inherits `MCTargetAsmParser`) and wire in `CGP1GenAsmMatcher.inc`.
- Ensure instruction defs have proper asm strings/operands for matcher.
- Register factories in target initialization.

4) Wire tools and test

- Verify: `clang --target=cgp1-unknown-unknown -c file.s -o file.o` succeeds.
- Use `llvm-objdump -d` to confirm encoding; `llvm-objcopy -O binary` to get flat bin.
- Add targeted MC tests (llvm-mc) for a few instructions and a small function.

5) Optional flat binary shortcut

- If needed, add `-filetype=binary` path in llvm-mc for CGP1 to emit flat `.bin` directly.