class Chip8
{
    public:
        Chip8();
        ~Chip8();

        bool drawFlag;

        void emulateCycle();
        void debugRender();
        void loadApplication(const char * filename);

    unsigned char gfx[64 * 32]; //black and white graphics with 2048 pixelsunsigned char gfx[64 * 32];
    unsigned char key[16]; //it has hex based 16 keys
    

    private:
        unsigned short opcode; //chip 8 has 35 opcode
        unsigned short I ; //index register
        unsigned short pc; //program counter
        unsigned short sp; //stack counter

        unsigned char V[16]; //it has 15 8-bit general register from V0 to V15 and last is carry flag
        unsigned char memory[4096]; //it has 4k memory in total
        unsigned short stack[16]; 
        
        unsigned char delay_timer; 
        unsigned char sound_timer;

        void init();
};