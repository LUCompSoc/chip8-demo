cmake_minimum_required(VERSION 3.10)
project(Chip8 C)

include_directories(include)
include_directories(lib/include)

# Include Raylib via system package or manually

add_executable(Chip8 
    src/main.c
    src/system.c
    src/cpu.c
)

target_link_libraries(Chip8 m pthread dl GL ${CMAKE_CURRENT_SOURCE_DIR}/lib/libraylib.so.5.5.0)