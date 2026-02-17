# CGP1 GPU Instruction Set Architecture (ISA)

This document describes the CGP1 GPU ISA based on the hardware team's specification.

## Instruction Format (32-bit)

```
31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
|  opcode (5)  | condition |  k/k_ra  |   sel   |    ra    |    rb    |    rc    |
```

### Field Descriptions

| Field | Bits | Description |
|-------|------|-------------|
| opcode | 31-27 (5 bits) | Instruction opcode |
| condition | 26-24 (3 bits) | Condition code for conditional execution |
| k/k_ra | 23-20 (4 bits) | Immediate or extended field |
| sel | 19-16 (4 bits) | Selection/modifier field |
| ra | 15-12 (4 bits) | Source/destination register A |
| rb | 11-8 (4 bits) | Source register B |
| rc | 7-4 (4 bits) | Source register C / destination |

## Opcodes

| Opcode (5-bit) | Binary | Instruction(s) |
|----------------|--------|----------------|
| 00000 | 0x00 | nop |
| 00001 | 0x01 | ploadf_l, loadf_h, ploadu_l, loadu_h, ploadaddr_l, loadaddr_h |
| 00010 | 0x02 | copy, fabs, fneg, fnabs |
| 00011 | 0x03 | fmulp2 |
| 00100 | 0x04 | fadd, fsub |
| 00101 | 0x05 | fmul |
| 00110 | 0x06 | fdiv |
| 00111 | 0x07 | fmin, fmax |
| 01000 | 0x08 | trunc, round, ceil, floor |
| 01001 | 0x09 | ftou8_ll, ftou8_lh, ftou8_hl, ftou8_hh |
| 01010 | 0x0A | ftou16_l, ftou16_h |
| 01011 | 0x0B | ftou32 |
| 01100 | 0x0C | u8tof_ll, u8tof_lh, u8tof_hl, u8tof_hh |
| 01101 | 0x0D | u16tof_l, u16tof_h |
| 01110 | 0x0E | u32tof |
| 01111 | 0x0F | fcomp_l, fcomp_le, fcomp_e, fcomp_ge, fcomp_g, fcomp_ne, fcomp_u |
| 10000 | 0x10 | add |
| 10001 | 0x11 | load_addr |
| 10010 | 0x12 | store_addr |
| 10011 | 0x13 | d_load, i_load, pop, load, load_d, load_i |
| 10100 | 0x14 | d_store, i_store, store, store_d, push, store_i |
| 10101 | 0x15 | jump, rcall |
| 10110 | 0x16 | goto, call |
| 10111 | 0x17 | ret |
| 11000 | 0x18 | stop_core |
| 11001 | 0x19 | irq |
| 11010 | 0x1A | thread_id_x/y/z, block_id_x/y/z, block_dim_x/y/z, grid_dim_x/y/z |
| 11011 | 0x1B | barrier |
| 11100 | 0x1C | write_pixel, clear_screen |

## Registers

The GPU uses a register file with registers addressed by 4-bit fields (ra, rb, rc).
This gives 16 general-purpose registers: r0-r15.

| Register | Encoding | Purpose |
|----------|----------|---------|
| r0 | 0000 | General purpose / Zero |
| r1 | 0001 | General purpose |
| r2 | 0010 | General purpose |
| r3 | 0011 | General purpose |
| r4 | 0100 | General purpose |
| r5 | 0101 | General purpose |
| r6 | 0110 | General purpose |
| r7 | 0111 | General purpose |
| r8 | 1000 | General purpose |
| r9 | 1001 | General purpose |
| r10 | 1010 | General purpose |
| r11 | 1011 | General purpose |
| r12 | 1100 | General purpose |
| r13 | 1101 | General purpose / Stack Pointer (SP) |
| r14 | 1110 | General purpose / Link Register (LR) |
| r15 | 1111 | General purpose / Program Counter (PC) |

## Instruction Details

### Floating-Point Arithmetic (opcode 00100-00110)

#### fadd - Floating-Point Add
```
fadd rc, ra, rb    ; rc = ra + rb
```
Opcode: 00100, sel determines add vs sub

#### fsub - Floating-Point Subtract
```
fsub rc, ra, rb    ; rc = ra - rb
```
Opcode: 00100, sel determines add vs sub

#### fmul - Floating-Point Multiply
```
fmul rc, ra, rb    ; rc = ra * rb
```
Opcode: 00101

#### fdiv - Floating-Point Divide
```
fdiv rc, ra, rb    ; rc = ra / rb
```
Opcode: 00110

### Memory Operations (opcode 01011-10011)

#### load - Load from memory
```
load rc, [ra + offset]    ; rc = mem[ra + offset]
```

#### store - Store to memory
```
store [ra + offset], rb   ; mem[ra + offset] = rb
```

### Control Flow (opcode 10100-11001)

#### jump - Unconditional jump
```
jump target    ; PC = target
```
Opcode: 10100

#### call - Function call
```
call target    ; LR = PC + 4, PC = target
```
Opcode: 10111

#### ret - Return from function
```
ret            ; PC = LR
```
Opcode: 11000

### Data Movement (opcode 00011)

#### copy - Register copy
```
copy rc, ra    ; rc = ra
```

#### fabs - Floating-point absolute value
```
fabs rc, ra    ; rc = |ra|
```

#### fneg - Floating-point negate
```
fneg rc, ra    ; rc = -ra
```

## Condition Codes (3-bit field)

| Code | Binary | Meaning |
|------|--------|---------|
| AL | 000 | Always (unconditional) |
| EQ | 001 | Equal |
| NE | 010 | Not equal |
| LT | 011 | Less than |
| LE | 100 | Less than or equal |
| GT | 101 | Greater than |
| GE | 110 | Greater than or equal |
| - | 111 | Reserved |

## Notes

- All instructions are 32-bit aligned
- Floating-point operations use IEEE 754 single-precision (float32)
- Memory is byte-addressable
- Big-endian byte order

---
*This document will be expanded as the hardware team provides more details.*
