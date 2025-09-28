#ifndef CHIP8_H
#define CHIP8_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "config.h"



#define CHIP8_MEMORY_SIZE 4096
#define CHIP8_DISPLAY_WIDTH 64
#define CHIP8_DISPLAY_HEIGHT 32


typedef enum { 
    RUNNING , 
    PAUSED ,
    STOPPED , 

}state_t ;
typedef struct {
    uint16_t opcode ; 
    uint16_t NNN ; 
    uint8_t NN ; 
    uint8_t N ; 
    uint8_t X ;
    uint8_t Y ;  
} instruction_t  ; 

typedef struct { 
    bool  display[CHIP8_DISPLAY_WIDTH * CHIP8_DISPLAY_HEIGHT]; // 64x32 pixel monochrome display
    bool keypad[16];        // Hexadecimal keypad 0x0-0xF
    uint8_t memory[CHIP8_MEMORY_SIZE];// 4K memory
    uint8_t V[16] ; // General purpose registers V0 to VF
    uint16_t I; // Index register
    uint16_t pc; // Program counter
    uint16_t stack[16]; // Stack for subroutine calls
    uint16_t *sp; // Stack pointer
    uint8_t delay_timer; // Delay timer
    uint8_t sound_timer; // Sound timer
    uint32_t pixel_color[64 * 32]; // Color of the pixels (for rendering)
    state_t state;
    const char *rom_name;
    instruction_t inst;
    char rom_name_copy[256];
    char save_filename[300];
 
} chip8_t;


bool init_chip8(chip8_t *chip8 ,const char rom_name[]) ; 
void run_intructions ( chip8_t *chip8 ) ; 
bool save_state ( chip8_t *chip8 , char *save_file ,size_t save_file_size , int slot) ;
bool load_state ( chip8_t *chip8 , char *save_file , size_t save_file_size , int slot ) ;

#endif // CHIP8_H