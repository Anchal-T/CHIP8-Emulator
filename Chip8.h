#ifndef CHIP8_H
#define CHIP8_H

#include <cstdint>

class Chip8
{
    public:
        Chip8();
        ~Chip8();

        bool drawFlag;

        void init();
        void emulateCycle();
        void debugRender();
        void loadApplication(const char * filename);

        uint8_t gfx[64 * 32];
        uint8_t key[16];

    private:
        uint16_t opcode;
        uint16_t I;
        uint16_t pc;
        uint16_t sp;

        uint8_t V[16];
        uint8_t memory[4096];
        uint16_t stack[16];
        
        uint8_t delay_timer; 
        uint8_t sound_timer;
};

#endif