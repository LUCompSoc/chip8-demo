#pragma once

#include "system.h"

enum CPUExecuteResult {
    ExecuteAdvance,             // This means we increment the Program Counter by 2 after running the instruction.
    ExecuteWithoutAdvancing,    // This means we don't change the Program Counter after (such as with the Jump instruction)
    ExecuteInvalidInstruction,  // This means the input opcode was invalid or not implemented.
};

void cpu_decrement_timers(struct System* system);
bool cpu_fetch_and_execute(struct System* system, u16 input);
int cpu_execute(struct System* system, u16 instruction, u16 input);

void cpu_instr_display(struct System* system, u16 instruction);

#define max(a,b)             \
({                           \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b;       \
})

#define min(a,b)             \
({                           \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b;       \
})