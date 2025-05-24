# CHIP-8 Emulator

A fully functional CHIP-8 emulator built in C++ using Raylib for graphics and input handling. This emulator can run classic CHIP-8 games and programs with accurate timing and display.

## Features

- **Complete CHIP-8 instruction set** - All 35 original CHIP-8 opcodes implemented
- **Raylib graphics** - Smooth 60 FPS rendering with scalable display
- **Configurable speed** - Adjustable CPU cycles per frame for different game requirements
- **Full keyboard support** - Complete 16-key hexadecimal keypad mapping
- **Sound support** - Beep sound when sound timer is active
- **ROM loading** - Support for standard CHIP-8 ROM files
- **Debug output** - Built-in debugging functions for development

## Controls

The CHIP-8 system uses a 16-key hexadecimal keypad. The keys are mapped to your keyboard as follows:

```
CHIP-8 Keypad    Keyboard
1 2 3 C          1 2 3 4
4 5 6 D          Q W E R
7 8 9 E          A S D F
A 0 B F          Z X C V
```

### Speed Controls
- The emulator speed can be controlled by passing the number of cycles per frame as a command-line argument
- Typical values range from 5-20 cycles per frame depending on the game

## Requirements

- **C++ Compiler** (g++ recommended)
- **Raylib** - Graphics and input library
- **Make** - For building the project

### Installing Raylib

On Ubuntu/Debian:
```bash
sudo apt update
sudo apt install libraylib-dev
```

On other systems, follow the [Raylib installation guide](https://github.com/raysan5/raylib#build-and-installation).

## Building

1. Clone or download this repository
2. Navigate to the project directory
3. Build using make:

```bash
make
```

This will create the `chip8emulator` executable.

## Usage

```bash
./chip8emulator <ROM_FILE_PATH> <CYCLES_PER_FRAME>
```

### Parameters

- `ROM_FILE_PATH`: Path to the CHIP-8 ROM file (e.g., `Tetris.ch8`)
- `CYCLES_PER_FRAME`: Number of CPU cycles to execute per frame (recommended: 10-15)

### Example

```bash
./chip8emulator Tetris.ch8 10
```

## ROM Files

The emulator supports standard CHIP-8 ROM files (typically with `.ch8` extension). You can find public domain CHIP-8 games and demos online. Some popular ones include:

- **Tetris** - Classic falling blocks game
- **Pong** - Simple paddle and ball game  
- **Space Invaders** - Shoot the aliens
- **Breakout** - Brick breaking game

## Technical Details

### CHIP-8 Specifications

- **Memory**: 4KB RAM (4096 bytes)
- **Display**: 64x32 monochrome pixels
- **Registers**: 16 8-bit registers (V0-VF)
- **Stack**: 16 levels for subroutines
- **Timers**: 60Hz delay and sound timers
- **Input**: 16-key hexadecimal keypad

### Emulator Implementation

- **CPU**: Fetch-decode-execute cycle with proper opcode handling
- **Graphics**: XOR-based sprite drawing with collision detection
- **Input**: Real-time keyboard polling and mapping
- **Timers**: 60Hz countdown timers for delay and sound
- **Memory Management**: Proper ROM loading and memory initialization

## File Structure

```
chip8_emulator/
├── chip8.cpp          # Core emulator implementation
├── Chip8.h            # Header file with class definition
├── main.cpp           # Raylib frontend and main loop
├── Makefile           # Build configuration
├── README.md          # This file
└── Tetris.ch8         # Example ROM file
```

## Development

### Debugging

The emulator includes a debug function that can print the current state:
- Program counter (PC)
- Current opcode
- Index register (I)
- Stack pointer (SP)
- All V registers
- Timer values
- Stack contents

To enable debugging, modify the code to call `myChip8.debugRender()` in the main loop.

### Modifying Speed

You can adjust the emulation speed by changing the `CYCLES_PER_FRAME` parameter:
- **Slower**: Use lower values (5-8) for games that run too fast
- **Faster**: Use higher values (15-20) for games that run too slow
- **Default**: 10 cycles per frame works well for most games

## Troubleshooting

### Common Issues

1. **Black screen**: Check if the ROM file exists and is valid
2. **Game too fast/slow**: Adjust the cycles per frame parameter
3. **No input response**: Verify keyboard mapping matches the game's expected controls
4. **Build errors**: Ensure Raylib is properly installed

### Performance Tips

- The emulator runs at 60 FPS by default
- Most CHIP-8 games expect 500-1000 Hz CPU speed
- Adjust `CYCLES_PER_FRAME` to match game requirements

## Contributing

Feel free to submit issues, feature requests, or pull requests. Some areas for improvement:

- Additional sound effects
- Save states
- Better debugging interface
- Performance optimizations
- Additional display filters/effects

## License

This project is open source. Feel free to use and modify as needed.

## Acknowledgments

- Based on the CHIP-8 emulator guide by [Austin Morlan](https://austinmorlan.com/posts/chip8_emulator/)
- Uses [Raylib](https://www.raylib.com/) for graphics and input
- CHIP-8 specification and test ROMs from the retro computing community

## References

- [CHIP-8 Wikipedia](https://en.wikipedia.org/wiki/CHIP-8)
- [Cowgod's CHIP-8 Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
- [CHIP-8 Test Suite](https://github.com/Timendus/chip8-test-suite)
