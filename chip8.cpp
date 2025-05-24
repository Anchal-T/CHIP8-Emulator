#include <bits/stdc++.h>
#include "Chip8.h"

unsigned char chip8_fontset[80] = 
{
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip8::Chip8()
{
    //empty
}

Chip8::~Chip8()
{
    //empty
}

void Chip8::init()
{
    pc = 0x200;
    opcode = 0;
    I = 0;
    sp = 0;

    for(int i = 0; i< 2048; i++){ //intializing gfx to 0
        gfx[i] = 0;
    }

    for(int i = 0; i < 16; i++){ //intializing stack to 0
        stack[i] = 0;
    }

    for(int i = 0; i < 16; i++){ //intializing key to 0
        key[i] = V[i] = 0;
    }

    for(int i = 0; i < 4096; i++){ //intializing memory to 0
        memory[i] = 0;
    }

    for(int i = 0; i < 80; i++){ //intializing memory to fontset
        memory[i] = chip8_fontset[i];
    }

    delay_timer = 0;
    sound_timer = 0;

    drawFlag = true;

    srand(time(NULL));
}

void Chip8::emulateCycle()
{
    opcode = memory[pc] << 8 | memory[pc + 1];

    switch (opcode & 0xF000)
    {
        case 0x0000:
            switch(opcode & 0x000F)
            {
                case 0x0000: // 0x00E0: Clear the display
                    for(int i = 0; i < 2048; i++)
                        gfx[i] = 0x0;
                    drawFlag = true;
                    pc += 2;
                    break;
                    
                case 0x000E: // 0x00EE: Return from a subroutine
                    sp--;
                    pc = stack[sp];
                    pc += 2;
                    break;
                    
                default:
                    printf("Unknown opcode [0x0000]: 0x%X\n", opcode);
                    break;
            }
            break;

        case 0x1000: // 0x1NNN: Jump to location nnn
            pc = opcode & 0x0FFF;
            break;

        case 0x2000: // 0x2NNN: Call subroutine at nnn
            stack[sp] = pc;
            sp++;
            pc = opcode & 0x0FFF;
            break;

        case 0x3000: // 0x3XNN: Skip next instruction if Vx = kk
            if(V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)){
                pc += 4;
            } else {
                pc += 2;
            }
            break;
            
        case 0x4000: // 0x4XNN: Skip next instruction if Vx != kk
            if(V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)){
                pc += 4;
            } else {
                pc += 2;
            }
            break;
            
        case 0x5000: // 0x5XY0: Skip next instruction if Vx = Vy
            if(V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4]){
                pc += 4;
            } else {
                pc += 2;
            }
            break;
            
        case 0x6000: // 0x6XNN: Set Vx = kk
            V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
            pc += 2;
            break;
            
        case 0x7000: // 0x7XNN: Set Vx = Vx + kk
            V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
            pc += 2;
            break;
            
        case 0x8000:
            switch(opcode & 0x000F){
                case 0x0000: // 0x8XY0: Set Vx = Vy
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                    
                case 0x0001: // 0x8XY1: Set Vx = Vx OR Vy
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] | V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                    
                case 0x0002: // 0x8XY2: Set Vx = Vx AND Vy
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] & V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                    
                case 0x0003: // 0x8XY3: Set Vx = Vx XOR Vy
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] ^ V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                    
                case 0x0004: // 0x8XY4: Set Vx = Vx + Vy, set VF = carry
                    if(V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8])){
                        V[0xF] = 1; // carry
                    } else {
                        V[0xF] = 0;
                    }
                    V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                    
                case 0x0005: // 0x8XY5: Set Vx = Vx - Vy, set VF = NOT borrow
                    if(V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4]){
                        V[0xF] = 1; // no borrow
                    } else {
                        V[0xF] = 0;
                    }
                    V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                    
                case 0x0006: // 0x8XY6: Set Vx = Vx SHR 1
                    V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x1;
                    V[(opcode & 0x0F00) >> 8] >>= 1;
                    pc += 2;
                    break;
                    
                case 0x0007: // 0x8XY7: Set Vx = Vy - Vx, set VF = NOT borrow
                    if(V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8]){
                        V[0xF] = 1;
                    } else {
                        V[0xF] = 0;
                    }
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                    break;
                    
                case 0x000E: // 0x8XYE: Set Vx = Vx SHL 1
                    V[0xF] = V[(opcode & 0x0F00) >> 8] >> 7;
                    V[(opcode & 0x0F00) >> 8] <<= 1;
                    pc += 2;
                    break;

                default:
                    printf ("Unknown opcode [0x8000]: 0x%X\n", opcode);
                    break;
            }
            break;

        case 0x9000: // 0x9XY0: Skip next instruction if Vx != Vy
            if(V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4]){
                pc += 4;
            } else {
                pc += 2;
            }
            break;

        case 0xA000: // 0xANNN: Set I = nnn
            I = opcode & 0x0FFF;
            pc += 2;
            break;

        case 0xB000: // 0xBNNN: Jump to location nnn + V0
            pc = (opcode & 0x0FFF) + V[0];
            break;

        case 0xC000: // 0xCXNN: Set Vx = random byte AND kk
            V[(opcode & 0x0F00) >> 8] = (rand() % 0xFF) & (opcode & 0x00FF);
            pc += 2;
            break;

        case 0xD000: // 0xDXYN: Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
        {
            unsigned short x = V[(opcode & 0x0F00) >> 8];
            unsigned short y = V[(opcode & 0x00F0) >> 4];
            unsigned short height = opcode & 0x000F;
            unsigned short pixel;

            V[0xF] = 0;
            for (int yline = 0; yline < height; yline++)
            {
                pixel = memory[I + yline];
                for(int xline = 0; xline < 8; xline++)
                {
                    if((pixel & (0x80 >> xline)) != 0)
                    {
                        if(gfx[(x + xline + ((y + yline) * 64))] == 1)
                        {
                            V[0xF] = 1;
                        }
                        gfx[x + xline + ((y + yline) * 64)] ^= 1;
                    }
                }
            }

            drawFlag = true;
            pc += 2;
        }
        break;

        case 0xE000:
            switch(opcode & 0x00FF)
            {
                case 0x009E: // 0xEX9E: Skip next instruction if key with the value of Vx is pressed
                    if(key[V[(opcode & 0x0F00) >> 8]] != 0)
                        pc += 4;
                    else
                        pc += 2;
                    break;

                case 0x00A1: // 0xEXA1: Skip next instruction if key with the value of Vx is not pressed
                    if(key[V[(opcode & 0x0F00) >> 8]] == 0)
                        pc += 4;
                    else
                        pc += 2;
                    break;

                default:
                    printf ("Unknown opcode [0xE000]: 0x%X\n", opcode);
                    break;
            }
            break;

        case 0xF000:
            switch(opcode & 0x00FF)
            {
                case 0x0007: // 0xFX07: Set Vx = delay timer value
                    V[(opcode & 0x0F00) >> 8] = delay_timer;
                    pc += 2;
                    break;

                case 0x000A: // 0xFX0A: Wait for a key press, store the value of the key in Vx
                {
                    bool keyPress = false;

                    for(int i = 0; i < 16; ++i)
                    {
                        if(key[i] != 0)
                        {
                            V[(opcode & 0x0F00) >> 8] = i;
                            keyPress = true;
                        }
                    }

                    if(!keyPress)
                        return;

                    pc += 2;
                }
                break;

                case 0x0015: // 0xFX15: Set delay timer = Vx
                    delay_timer = V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                    break;

                case 0x0018: // 0xFX18: Set sound timer = Vx
                    sound_timer = V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                    break;

                case 0x001E: // 0xFX1E: Set I = I + Vx
                    if(I + V[(opcode & 0x0F00) >> 8] > 0xFFF)
                        V[0xF] = 1;
                    else
                        V[0xF] = 0;
                    I += V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                    break;

                case 0x0029: // 0xFX29: Set I = location of sprite for digit Vx
                    I = V[(opcode & 0x0F00) >> 8] * 0x5;
                    pc += 2;
                    break;

                case 0x0033: // 0xFX33: Store BCD representation of Vx in memory locations I, I+1, and I+2
                    memory[I]     = V[(opcode & 0x0F00) >> 8] / 100;
                    memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
                    memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
                    pc += 2;
                    break;

                case 0x0055: // 0xFX55: Store registers V0 through Vx in memory starting at location I
                    for (int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i)
                        memory[I + i] = V[i];

                    I += ((opcode & 0x0F00) >> 8) + 1;
                    pc += 2;
                    break;

                case 0x0065: // 0xFX65: Read registers V0 through Vx from memory starting at location I
                    for (int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i)
                        V[i] = memory[I + i];

                    I += ((opcode & 0x0F00) >> 8) + 1;
                    pc += 2;
                    break;

                default:
                    printf ("Unknown opcode [0xF000]: 0x%X\n", opcode);
                    break;
            }
            break;

        default:
            printf ("Unknown opcode: 0x%X\n", opcode);
            break;
    }

    // Update timers
    if(delay_timer > 0)
        --delay_timer;

    if(sound_timer > 0)
    {
        if(sound_timer == 1)
            printf("BEEP!\n");
        --sound_timer;
    }
}

void Chip8::loadApplication(const char * filename)
{
    FILE* pFile = fopen(filename, "rb");
    if (pFile == NULL)
    {
        printf("Error: Could not open file %s\n", filename);
        return;
    }

    // Get file size
    fseek(pFile, 0, SEEK_END);
    long lSize = ftell(pFile);
    rewind(pFile);

    // Allocate memory to contain the file
    char* buffer = (char*)malloc(sizeof(char) * lSize);
    if (buffer == NULL)
    {
        printf("Error: Memory allocation failed\n");
        fclose(pFile);
        return;
    }

    // Copy the file into the buffer
    size_t result = fread(buffer, 1, lSize, pFile);
    if (result != (long unsigned int)lSize)
    {
        printf("Error: Reading file failed\n");
        free(buffer);
        fclose(pFile);
        return;
    }

    // Copy buffer to Chip8 memory
    if((4096-512) > lSize)
    {
        for(int i = 0; i < lSize; ++i)
            memory[i + 512] = buffer[i];   // Load the game into memory starting at 0x200 (512)
    }
    else
    {
        printf("Error: ROM too big for memory\n"); 
    }

    // Clean up
    fclose(pFile);
    free(buffer);
}

void Chip8::debugRender()
{
    // Print state for debugging
    printf("PC: 0x%X, Opcode: 0x%X, I: 0x%X, SP: 0x%X\n", pc, opcode, I, sp);
    printf("V registers: ");
    for(int i = 0; i < 16; i++)
    {
        printf("V%X: 0x%X ", i, V[i]);
    }
    printf("\n");
    printf("Timers - Delay: %d, Sound: %d\n", delay_timer, sound_timer);
    printf("Stack: ");
    for(int i = 0; i < sp; i++)
    {
        printf("0x%X ", stack[i]);
    }
    printf("\n\n");
}
