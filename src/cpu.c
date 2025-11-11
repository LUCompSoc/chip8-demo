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

void cpu_fetch_and_execute(struct System *system, u16 input) {
    // Get the next 16-bit instruction according to the program counter.
    u16 instruction = ((u16)system->ram[system->pc] << 8) + system->ram[system->pc + 1];

    // Increment the program counter.
    system->pc += 2;
    
    // Execute the instruction. If it returns false, such as if it is
    // a halting instruction, set the PC back to what it was.
    if (!cpu_execute(system, instruction, input)) {
        system->pc -= 2;
    }
}

int cpu_execute(struct System *system, u16 instruction, u16 input) {
    // Isolate the first nibble (4 bits)
    u8 first_nibble = instruction >> 12;

    switch (first_nibble) {
        case 0x0:
            if (instruction == 0x00E0) {
                // Clear screen (00E0)
                memset(system->display, 0, sizeof(system->display));
            }
            break;
        case 0x1:
            // TODO: Jump to instruction, set PC to NNN (1NNN)
            break;
        case 0x6:
            // TODO: Set register VX to NN (7XNN)
            break;
        case 0x7:
            // TODO: Add NN to VX (6XNN)
            break;
        case 0xA:
            // TODO: Set index register to NNN (ANNN)
            break;
        case 0xD:
            // TODO: Display sprite (DXYN)
            break;
    }

    return ExecuteInvalidInstruction;
}

// 
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
