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
    case 0x000:
        switch(opcode & 0x000F)
            case 0x0000:
                for(int i = 0; i < 2048; i++) //clear screen
                    gfx[i] = 0x0;
                     drawFlag = true;
                      pc += 2;
            break;
            case 0x000E:
                sp--;
                 pc = stack[sp];
                  pc += 2;
            break;

            case 0x1000:
                pc = opcode & 0x0FFF;
            break;

            case 0x2000:
                stack[sp] = pc;
                 sp++;
                  pc = opcode & 0x0FFF;
            break;

            case 0x3000:
                if(V[opcode & 0x0FFF] == (opcode & 0x00FF)){
                    pc += 4;
                }          
                else{
                    pc += 2;
                } 
                 break;
            case 0x4000:
                if(V[opcode & 0x00FF] != (opcode & 0x00FF)){
                    pc += 4;
                }          
                else{
                    pc += 2;
                } 
            break;
            case 0x5000:
                if(V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4]){
                    pc += 4;
                }
                else{
                    pc += 2;
                }
            break;
            case 0x6000:
                V[(opcode & 0x0F00)] = opcode & 0x00FF;
                 pc += 2;
            break;  
            case 0x7000:
                V[(opcode & 0x0F00)] += opcode & 0x00FF;
                 pc += 2;
                  break;
            case 0x8000:
                switch(opcode & 0x000F){
                    case 0x0000:
                        V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
                         pc += 2;
                    break;
                    case 0x0001:
                        V[(opcode & 0x00F0) >> 4] = V[(opcode & 0x00F0) >> 4] | V[(opcode & 0x0F00) >> 8];
                         pc += 2;
                    break;
                    case 0x0002:
                        V[(opcode & 0x00F0) >> 4] = V[(opcode & 0x00F0) >> 4] & V[(opcode & 0x0F00) >> 8];
                         pc += 2;
                    break;
                    case 0x0003:
                        V[(opcode & 0x00F0) >> 4] = V[(opcode & 0x00F0) >> 4] ^ V[(opcode & 0x0F00) >> 8];
                         pc += 2;
                    break;
                    case 0x0004:
                        if(V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8])){
                            V[0xF] = 1;
                        }
                        else{
                            V[0xF] = 0;
                        }
                        V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
                         pc += 2;
                    break;
                    case 0x0005:
                        if(V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8]){
                            V[0xF] = 1;
                        }
                        else{
                            V[0xF] = 0;
                        }
                        V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
                         pc += 2;
                    break;
                    case 0x0006:
                        V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x1;
                        V[(opcode & 0x0F00) >> 8] >= 1;
                        pc += 2;
                    break;
                    case 0x0007:
                        if(V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4])	
						V[0xF] = 0; 
					else
						V[0xF] = 1;
					    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];				
					    pc += 2;
				    break;
                    case 0x000E:
                        V[0xF] = V[(opcode & 0x0F00) >> 8] >> 7;
					    V[(opcode & 0x0F00) >> 8] <<= 1;
					    pc += 2;
				    break;

				    default:
					    printf ("Unknown opcode [0x8000]: 0x%X\n", opcode);
			        }
		        break;

                case 0x9000:
                    
                }    
    }
