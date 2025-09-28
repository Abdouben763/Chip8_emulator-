# 🎮 CHIP-8 Emulator

A modern, feature-rich CHIP-8 emulator written in C using SDL2.

![Build Status](https://img.shields.io/badge/build-passing-brightgreen)
![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows%20%7C%20macOS-blue)
![Language](https://img.shields.io/badge/language-C99-orange)

## ✨ Features

- **Complete CHIP-8 instruction set** - All 35 opcodes implemented
- **Advanced save/load system** - 4 save slots per ROM with automatic filename generation
- **High-quality graphics** - Smooth SDL2 rendering with customizable display
- **Authentic audio** - Classic CHIP-8 beep sound
- **Flexible controls** - AZERTY keyboard mapping (QWERTY instructions included)
- **Pause/Resume functionality** - Space to pause, M to reset
- **Clean build system** - Modern Makefile with colored output

## 🚀 Quick Start

### Prerequisites
```bash
# Ubuntu/Debian
sudo apt install libsdl2-dev gcc make

# Fedora/RHEL
sudo dnf install SDL2-devel gcc make

# Arch Linux
sudo pacman -S sdl2 gcc make

# macOS (with Homebrew)
brew install sdl2 gcc make

# Windows
# Option 1: MSYS2 (Recommended)
# 1. Install MSYS2 from https://www.msys2.org/
# 2. Open MSYS2 terminal and run:
pacman -S mingw-w64-x86_64-SDL2 mingw-w64-x86_64-gcc make
 
```

### Build & Run
```bash
# Clone the repository
git clone https://github.com/yourusername/chip8-emulator.git
cd chip8-emulator

# Build the emulator
make

# Run with a ROM
./chip8 roms/Brick.ch8

# Or build and run in one command
make run
```

## 🎯 Usage

### Command Line
```bash
./chip8 <rom_file>
```

### Controls

#### System Controls
- **ESC** - Quit emulator
- **SPACE** - Pause/Resume
- **M** - Reset emulator

#### Save/Load States
- **F1-F4** - Save to slots 1-4
- **F5-F8** - Load from slots 1-4

#### CHIP-8 Keypad (AZERTY Layout)
```
CHIP-8 Keypad       AZERTY keyboard
┌───┬───┬───┬───┐  ┌───┬───┬───┬───┐
│ 1 │ 2 │ 3 │ C │  │ 1 │ 2 │ 3 │ 4 │
├───┼───┼───┼───┤  ├───┼───┼───┼───┤
│ 4 │ 5 │ 6 │ D │  │ A │ Z │ E │ R │
├───┼───┼───┼───┤  ├───┼───┼───┼───┤
│ 7 │ 8 │ 9 │ E │  │ Q │ S │ D │ F │
├───┼───┼───┼───┤  ├───┼───┼───┼───┤
│ A │ 0 │ B │ F │  │ W │ X │ C │ V │
└───┴───┴───┴───┘  └───┴───┴───┴───┘
```

> **For QWERTY users:** The controls remain the same physical keys, just different labels.

## 📁 Project Structure

```
chip8-emulator/
├── src/                    # Source files
│   ├── main.c             # Main entry point and game loop
│   ├── chip8.c            # CHIP-8 CPU implementation
│   ├── chip8_sdl.c        # SDL graphics and audio
│   ├── input.c            # Input handling and save states
│   ├── timer.c            # Timer management (60Hz)
│   └── config.c           # Configuration settings
├── include/               # Header files
│   ├── chip8.h            # CHIP-8 system structures
│   ├── sdl.h              # SDL wrapper definitions
│   ├── input.h            # Input function declarations
│   ├── timer.h            # Timer function declarations
│   └── config.h           # Configuration definitions
├── roms/                  # Sample ROM files
│   ├── Brick.ch8          # Breakout game
│   ├── Tetris.ch8         # Tetris implementation
│   └── IBM-Logo.ch8       # IBM logo display
├── docs/                  # Documentation
│   └── chip8ref.pdf       # CHIP-8 reference manual
├── Makefile               # Build system
├── .gitignore             # Git ignore rules
└── README.md              # This file
```

## 🔧 Build System

The project uses a modern Makefile with the following targets:

```bash
make           # Build the emulator
make run       # Build and run with Brick.ch8
make clean     # Remove build files
make help      # Show available targets
```

## 🎮 Compatible ROMs

This emulator is compatible with all standard CHIP-8 ROMs.

> 💡 **For Roms** Check out this excellent collection: [CHIP-8 ROM Archive](https://github.com/kripod/chip8-roms) by @kripod

## 🛠️ Technical Details

### CHIP-8 Specifications
- **Memory:** 4KB RAM (0x000-0xFFF)
- **Display:** 64×32 monochrome pixels
- **Registers:** 16 8-bit general purpose (V0-VF)
- **Stack:** 16 levels for subroutines
- **Timers:** 60Hz delay and sound timers
- **Input:** 16-key hexadecimal keypad

### Implementation Features
- **Accurate timing** - 60 FPS with configurable instruction rate
- **Save states** - Complete system state preservation
- **Memory safety** - Bounds checking and error handling
- **Cross-platform** - Runs on Linux, Windows, and macOS

## 📝 Save State System

The emulator features an advanced save state system:

- **4 slots per ROM** - Each ROM has independent save slots
- **Automatic naming** - Saves as `romname_slot1.bin`, etc.
- **Complete state** - Preserves memory, registers, and display
- **Instant access** - F1-F8 keys for quick save/load


## 📚 References

- [CHIP-8 Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
- [CHIP-8 Wikipedia](https://en.wikipedia.org/wiki/CHIP-8)
- [SDL2 Documentation](https://wiki.libsdl.org/)

---

**Made by Abderrahamane benchikh**
