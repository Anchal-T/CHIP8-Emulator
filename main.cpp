
#include "raylib.h"
#include "Chip8.h"
#include <iostream>
#include <string>

// CHIP-8 display dimensions
const int CHIP8_WIDTH = 64;
const int CHIP8_HEIGHT = 32;

// Window dimensions (scaled)
const int SCREEN_SCALE = 10;
const int SCREEN_WIDTH = CHIP8_WIDTH * SCREEN_SCALE;
const int SCREEN_HEIGHT = CHIP8_HEIGHT * SCREEN_SCALE;

// Number of CHIP-8 cycles to run per frame. Adjust for speed.
Chip8 myChip8; // Global Chip8 instance

// CHIP-8 Keypad to Raylib Keyboard mapping
// CHIP-8: 1 2 3 C | Keyboard: 1 2 3 4
//         4 5 6 D |           Q W E R
//         7 8 9 E |           A S D F
//         A 0 B F |           Z X C V
int keymap[16] = {
    KEY_X,    // 0
    KEY_ONE,  // 1
    KEY_TWO,  // 2
    KEY_THREE,// 3
    KEY_Q,    // 4
    KEY_W,    // 5
    KEY_E,    // 6
    KEY_A,    // 7
    KEY_S,    // 8
    KEY_D,    // 9
    KEY_Z,    // A (10)
    KEY_C,    // B (11)
    KEY_FOUR, // C (12)
    KEY_R,    // D (13)
    KEY_F,    // E (14)
    KEY_V     // F (15)
};

void ProcessInput() {
    for (int i = 0; i < 16; i++) {
        if (IsKeyDown(keymap[i])) {
            myChip8.key[i] = 1;
        } else {
            myChip8.key[i] = 0;
        }
    }
}

void UpdateDisplay() {
    BeginDrawing();
    ClearBackground(BLACK);

    for (int y = 0; y < CHIP8_HEIGHT; ++y) {
        for (int x = 0; x < CHIP8_WIDTH; ++x) {
            if (myChip8.gfx[(y * CHIP8_WIDTH) + x] == 1) { // CHIP-8 pixels are 0 or 1
                DrawRectangle(x * SCREEN_SCALE, y * SCREEN_SCALE, SCREEN_SCALE, SCREEN_SCALE, WHITE);
            }
        }
    }

    EndDrawing();
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <ROM_FILE_PATH>" << std::endl;

        std::cerr << "Usage: " << argv[1] << " <Target cycle>" << std::endl;
        return 1;
    }

    const char* romFilename = argv[1];
    int CYCLES_PER_FRAME = std::atoi(argv[2]);

    myChip8.init();
    myChip8.loadApplication(romFilename); 

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chip-8 Emulator - Raylib");
    SetTargetFPS(60); // Target FPS

    while (!WindowShouldClose()) {
        ProcessInput();

        for(int i = 0; i < CYCLES_PER_FRAME; ++i) {
            myChip8.emulateCycle();
        }

        if (myChip8.drawFlag) {
            UpdateDisplay();
            myChip8.drawFlag = false;
        }
    }

    CloseWindow();
    return 0;
}
