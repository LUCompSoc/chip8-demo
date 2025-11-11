#pragma once

#include "system.h"

enum CPUExecuteResult {
    ExecuteNormal,
    ExecuteBlock,
    ExecuteInvalidInstruction,
};

void cpu_decrement_timers(struct System* system);
void cpu_fetch_and_execute(struct System* system, u16 input);
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