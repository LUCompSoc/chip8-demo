#include "raylib.h"

#include <stdio.h>
#include <stdlib.h>
#include "system.h"
#include "cpu.h"

#define PIXEL_SCALE 16

struct FileReadResult {
    bool success;
    u8* buffer;
    int length;
};

struct FileReadResult read_whole_file(const char* path) {
    struct FileReadResult result;

    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        perror("Couldn't read from the file!");
        result.success = false;
        return result;
    }

    // Find the length of the file
    fseek(file, 0, SEEK_END);
    long len = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Initialize the buffer
    char* buffer = (char*)malloc(len);
    if (buffer == NULL) {
        perror("Couldn't initialize the memory!");
        result.success = false;
        return result;
    }
    fread(buffer, 1, len, file);
    fclose(file);
    
    result.success = true;
    result.buffer = buffer;
    result.length = len;
    
    return result;
}

void show_display(const struct System* system) {
    for (int y = 0; y < DISPLAY_HEIGHT; y++) {
        for (int row_byte_id = 0; row_byte_id < DISPLAY_WIDTH / 8; row_byte_id++) {
            u8 row_byte = system->display[y * DISPLAY_WIDTH / 8 + row_byte_id];

            for (int row_byte_entry = 0; row_byte_entry < 8; row_byte_entry++) {
                int x = (int)row_byte_id * 8 + row_byte_entry;

                if ((row_byte & (128 >> row_byte_entry)) != 0) {
                    DrawRectangle(x * PIXEL_SCALE, y * PIXEL_SCALE, PIXEL_SCALE, PIXEL_SCALE, WHITE);
                } else {
                    DrawRectangle(x * PIXEL_SCALE, y * PIXEL_SCALE, PIXEL_SCALE, PIXEL_SCALE, BLACK);
                }
            }
        }
    }
}

int main(void)
{
    char* program;
    int program_length;
    {
        struct FileReadResult read_file = read_whole_file("./programs/IBM_Logo.ch8");
        if (!read_file.success) {
            return 1;
        }
        program = read_file.buffer;
        program_length = read_file.length;
    }

    struct System* system = malloc(sizeof(struct System));

    if (!init_system(system, program, program_length)) {
        return 1;
    }

    // Create the Raylib window with a width, height and title
    InitWindow(DISPLAY_WIDTH  * PIXEL_SCALE, 
               DISPLAY_HEIGHT * PIXEL_SCALE, 
               "Chip8 Emulator");
    
    // Set to VSync
    SetWindowState(FLAG_VSYNC_HINT);
    
    // This is the game loop
    while (!WindowShouldClose())
    {
        cpu_decrement_timers(system);
        for (int i = 0; i < 700; i++) {
            cpu_fetch_and_execute(system, 0);
        }

        BeginDrawing();
        //ClearBackground(BLACK);
        show_display(system);
        EndDrawing();
    }

    CloseWindow();

    free(system);

    return 0;
}