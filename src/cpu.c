#include "cpu.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void cpu_decrement_timers(struct System *system) {
    if (system->sound_timer > 0) {
        system->sound_timer--;
    }

    if (system->delay_timer > 0) {
        system->delay_timer--;
    }
}

bool cpu_fetch_and_execute(struct System *system, u16 input) {
    // Get the next 16-bit instruction according to the program counter.
    u16 instruction = ((u16)system->ram[system->pc] << 8) + system->ram[system->pc + 1];

    // Increment the program counter.
    system->pc += 2;
    
    // Execute the instruction. If it returns false, such as if it is
    // a halting instruction, set the PC back to what it was.
    int result = cpu_execute(system, instruction, input);

    switch (result) {
        case ExecuteAdvance:
            break;
        case ExecuteWithoutAdvancing:
            system->pc -= 2;
            break;
        case ExecuteInvalidInstruction:
            return false;
    }

    return true;
}

int cpu_execute(struct System *system, u16 instruction, u16 input) {
    // Isolate the first nibble (4 bits) using bit-shifting.
    u8 first_nibble = instruction >> 12;

    // For just the basic instructions we only need to look at the first nibble
    // of the opcode to figure out which instruction to execute
    switch (first_nibble) {
        case 0x0:
            if (instruction == 0x00E0) {
                // Clear screen (00E0)
                memset(system->display, 0, sizeof(system->display));
            }
            return ExecuteAdvance;
        case 0x1:
            // TODO: Jump to instruction, set PC to NNN (1NNN)
            
            return ExecuteAdvance;
        case 0x6:
            // TODO: Set register VX to NN (7XNN)
            
            return ExecuteAdvance;
        case 0x7:
            // TODO: Add NN to VX (6XNN)
            
            return ExecuteAdvance;
        case 0xA:
            // TODO: Set index register I to NNN (ANNN)
            
            return ExecuteAdvance;
        case 0xD:
            // Display sprite (DXYN)
            cpu_instr_display(system, instruction);
            return ExecuteAdvance;
    }

    return ExecuteInvalidInstruction;
}

void cpu_instr_display(struct System *system, u16 instruction) {
    u8 h = instruction & 0x000F;

    u8 sx = system->registers[((instruction & 0x0F00) >> 8)] % DISPLAY_WIDTH;
    u8 sy = system->registers[((instruction & 0x00F0) >> 4)] % DISPLAY_HEIGHT;

    system->registers[0xF] = 0;

    u8 byte_offset = sx & 7;

    u16 i = 0;

    for (u16 y = sy; y < min(sy+h, DISPLAY_HEIGHT); y++) {
        u16 start_display_index = y * (DISPLAY_WIDTH / 8) + (u16)sx / 8;

        u8 sprite_row = system->ram[system->I + i];
        u8 dest_row   = (system->display[start_display_index] << byte_offset) + (system->display[start_display_index + 1] >> (8 - byte_offset));

        u8 xor_row = sprite_row ^ dest_row;

        if (xor_row != (sprite_row | dest_row)) {
            system->registers[0xF] = 1;
        }

        system->display[start_display_index] &= 255 << (8 - byte_offset);
        system->display[start_display_index] |= xor_row >> byte_offset;

        // Clip at the right side of the screen
        if ((start_display_index + 1) % (DISPLAY_WIDTH / 8) != 0) {
            system->display[start_display_index + 1] &= 255 >> byte_offset;
            system->display[start_display_index + 1] |= xor_row << (8 - byte_offset);
        }

        i += 1;
    }
}
