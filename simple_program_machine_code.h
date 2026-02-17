// CGP1 GPU Machine Code - simple_program.c
// Generated with CORRECT ISA opcodes
// ADD=10000, LOAD=10011, STORE=10100, RET=10111

#ifndef SIMPLE_PROGRAM_MACHINE_CODE_H
#define SIMPLE_PROGRAM_MACHINE_CODE_H

#include <stdint.h>

#define SIMPLE_PROGRAM_NUM_INSTRUCTIONS 8

// Machine code as 32-bit words (big-endian)
static const uint32_t simple_program_code[] = {
    0xA000D100,  // 0: loadi $r1, 0
    0xA000D200,  // 1: store [$r13 + 12], $r1
    0xA000D200,  // 2: loadi $r2, 10
    0x9800D020,  // 3: store [$r13 + 8], $r2
    0x9800D030,  // 4: loadi $r2, 20
    0x80002320,  // 5: store [$r13 + 4], $r2
    0xA000D200,  // 6: load $r2, [$r13 + 8]
    0xB8000000,  // 7: load $r3, [$r13 + 4]
};

#endif // SIMPLE_PROGRAM_MACHINE_CODE_H
