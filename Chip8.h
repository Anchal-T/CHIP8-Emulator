#ifndef CHIP8_H
#define CHIP8_H

#include <cstdint>

class Chip8
{
    public:
        Chip8();
        ~Chip8();

        bool drawFlag;// Flag to indicate when screen needs to be redrawn

        void init();// Initialize the system
        void emulateCycle();// Execute one cycle of the emulator
        void debugRender();// Debug function to render current state
        void loadApplication(const char * filename);// Load ROM file into memory

        uint8_t gfx[64 * 32];  // Graphics buffer (64x32 pixels)
        uint8_t key[16];       // Keypad state (16 keys)

    private:
        uint16_t opcode;       // Current instruction being executed
        uint16_t I;            // Index register
        uint16_t pc;           // Program counter
        uint16_t sp;           // Stack pointer

        uint8_t V[16];         // CPU registers V0-VF
        uint8_t memory[4096];  // System memory (4KB)
        uint16_t stack[16];    // Stack for subroutine calls
        
        uint8_t delay_timer;   // Delay timer register
        uint8_t sound_timer;   // Sound timer register
};

#endif