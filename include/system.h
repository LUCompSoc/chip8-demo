#pragma once

#include <stdbool.h>

#define DISPLAY_WIDTH  64
#define DISPLAY_HEIGHT 32

typedef unsigned char u8;
typedef unsigned short u16;

#define V0 0;
#define VF 15;

struct Stack {
    u8 data[16];
    int size;
};

struct System {
    u8 ram[4096];
    u8 display[DISPLAY_WIDTH*DISPLAY_HEIGHT/8+1];
    u16 pc;
    u16 I;
    struct Stack stack;
    u8 delay_timer;
    u8 sound_timer;
    u8 registers[16];

    u16 last_input;
};

enum InputButton {
    BTN_1,
    BTN_2,
    BTN_3,
    BTN_4,
    BTN_5,
    BTN_6,
    BTN_7,
    BTN_8,
    BTN_9,
    BTN_0,
    BTN_A,
    BTN_B,
    BTN_C,
    BTN_D,
    BTN_E,
    BTN_F,
};

extern const u8 font[80];

#define FONT_START_INDEX    0x50
#define PROGRAM_START_INDEX 0x200

bool init_system(struct System* system, u8* program, int program_size);