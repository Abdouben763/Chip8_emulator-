/**
 * @file chip8.c
 * @brief CHIP-8 Emulator Core Implementation
 * @author Abderrahmane Benchikh
 * @date 2025
 * 
 * This file contains the core implementation of the CHIP-8 emulator,
 * including initialization, instruction execution, and state saving/loading.
 * It adheres to the C17 standard and uses SDL2 for logging.
 */


#include "chip8.h"

// Initialize CHIP-8 system and load ROM
bool init_chip8 (chip8_t *chip8 , const char rom_name[]) {
    const uint32_t entry_point = 0x200 ; 
     
    // Built-in hexadecimal font set (0-F), each character is 4x5 pixels
    const uint8_t font[] = {
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
    } ;
    // Clear all memory and registers
    memset ( chip8 , 0 , sizeof ( chip8_t ) ) ;
    // Load font set into memory (0x50-0x9F)
    memcpy (&chip8->memory[0], font , sizeof(font )) ; 
    
    // Open ROM file
    FILE *rom = fopen(rom_name , "rb") ; 
     if (!rom) { 
        SDL_Log ("Rom file %s is invalid\n" ,rom_name ) ; 
            return false   ;
     }
    // Get ROM size and validate it fits in memory
    fseek ( rom , 0 , SEEK_END) ; 
    const size_t rom_size = ftell(rom) ; 
    const size_t max_size = sizeof ( chip8->memory ) - entry_point ; 
    rewind(rom) ; 

    if (rom_size > max_size) {
        SDL_Log("Rom file %s size is too big, rom size : %zu, max size : %zu\n " , rom_name , rom_size , max_size) ; 
        fclose(rom);
        return false ; 
    }
    // Load ROM into memory starting at 0x200
    if (fread(&chip8->memory[entry_point], rom_size , 1 , rom )!= 1) {
        SDL_Log ("Could not read rom %s \n" , rom_name) ; 
        fclose(rom);
        return false;
    } 


     fclose(rom) ;
    

    // set chip8 // config 
    chip8->state = RUNNING ; 
    chip8->pc = entry_point ; 
    chip8->sp = &chip8->stack[0];  // Initialize stack pointer to beginning of stack
    chip8->rom_name = rom_name  ; 



    return true  ; 
}
// Generate save filename based on ROM name and slot number
static void prepare_save_filename(chip8_t *chip8, char *save_file, size_t save_file_size, int slot) {
    // Copy ROM name to working buffer
    strncpy(chip8->rom_name_copy, chip8->rom_name, sizeof(chip8->rom_name_copy) - 1);
    chip8->rom_name_copy[sizeof(chip8->rom_name_copy) - 1] = '\0';

    // Remove file extension if present
    char *dot = strrchr(chip8->rom_name_copy, '.');
    if (dot) *dot = '\0';

    // Create filename: "romname_slotN.bin"
    snprintf(save_file, save_file_size, "%s_slot%d.bin", chip8->rom_name_copy, slot);
}

// Save current CHIP-8 state to file
bool save_state ( chip8_t *chip8 , char *save_file,size_t save_file_size ,  int slot ) { 
    prepare_save_filename(chip8, save_file, save_file_size, slot);

    FILE *file = fopen(save_file , "wb") ; 
    if (!file) { 
        SDL_Log("Could not open file %s for writing\n" , save_file) ;
        return false ; 
    }
    // Write entire system state as binary data
    if ( fwrite ( chip8 , sizeof ( chip8_t ) , 1 , file) != 1 ) { 
        SDL_Log ("Could not write to file %s\n" , save_file) ; 
        fclose(file) ; 
        return false ; 
    }

    fclose(file) ;
    return true ;
}



// Load CHIP-8 state from save file
bool load_state ( chip8_t *chip8 ,  char *save_file , size_t save_file_size , int slot ) { 
    prepare_save_filename(chip8, save_file, save_file_size, slot);

    FILE *file = fopen(save_file , "rb") ; 
    if (!file) { 
        SDL_Log("Could not open file %s for reading\n" , save_file) ;
        return false ; 
    }
    // Load entire system state from binary data
    if ( fread ( chip8 , sizeof ( chip8_t ) , 1 , file) != 1 ) { 
        SDL_Log ("Could not read from file %s\n" , save_file) ; 
        fclose(file) ; 
        return false ; 
    }
    fclose(file) ;
    return true ;
}

void run_intructions ( chip8_t *chip8 ) { 
    bool carry ;
    chip8->inst.opcode= (chip8->memory[chip8->pc] << 8 ) | chip8->memory[chip8->pc+1 ] ; 
    chip8->pc += 2 ; 
    
    // Single out the opcodes 
    chip8->inst.NNN = chip8->inst.opcode &  0x0FFF ; 
    chip8->inst.NN = chip8->inst.opcode & 0x00FF ; 
    chip8->inst.N = chip8->inst.opcode & 0x000F ; 
    chip8->inst.X = (chip8->inst.opcode >> 8 ) & 0x000F ;
    chip8->inst.Y = (chip8->inst.opcode >> 4 ) & 0x000F ;


    switch ( (chip8->inst.opcode >> 12) & 0x000F ) 

    {   
        case 0x00 : 
            if(chip8->inst.NN == 0xE0) { 
                memset(&chip8->display[0], false, sizeof chip8->display);
            } 
            else if ( chip8->inst.NN == 0xEE) { 
                chip8->pc = *--chip8->sp ; 
            }
            break;

        case 0x01 : 
            chip8->pc = chip8->inst.NNN ; 
            break;

        case 0x02 :  
            // 0x2NNN: Call subroutine at NNN
            *chip8->sp++ = chip8->pc ;
            chip8->pc = chip8->inst.NNN ;
            break; 

        case 0x03 :
            // 0x3XNN: Skip next instruction if VX == NN
            
            if(chip8->V[chip8->inst.X] == chip8->inst.NN) { 
                chip8->pc += 2 ; 
            }
            break;
    

        case 0x04 :
            // 0x4XNN: Skip next instruction if VX != NN 
            if (chip8->V[chip8->inst.X] != chip8->inst.NN) {
                chip8->pc += 2;
            }
            break;

        case 0x05 :  
            // 0x5XY0: Skip next instruction if VX == VY
            if (chip8->inst.N != 0) break ; 
            if (chip8->V[chip8->inst.X] == chip8->V[chip8->inst.Y]) { 
                chip8->pc += 2  ;
            }
            break;
        
        case 0x06 :
            // 0x6XNN: Set VX to NN
            chip8->V [ chip8->inst.X] = chip8->inst.NN ; 
            break;
        case (0x07) : 
            // 0x7XNN: Set VX += NN
            chip8->V[chip8->inst.X] += chip8->inst.NN ; 
            break;
            
        case 0x08 :
            switch (chip8->inst.N) 
            {
                case 0x00:
                    chip8->V[chip8->inst.X] = chip8->V[chip8->inst.Y ] ;    
                    break;
                case 0x01 :
                    chip8->V[chip8->inst.X] |= chip8->V[chip8->inst.Y] ;   
                    chip8->V[0x0F] =0 ; // carry flag
                    break;
                case 0x02 :
                    chip8->V[chip8->inst.X] &= chip8->V[chip8->inst.Y] ; 
                    break;
                
                case 0x03 : 
                    chip8->V[chip8->inst.X] ^= chip8->V[chip8->inst.Y] ;
                    chip8->V[0x0F] = 0 ; // carry flag
                    break; 
                case 0x04 : 
                 // 0x8XY4: Set register VX += VY, set VF to 1 if carry, 0 if not 
                    carry = ((uint16_t)(chip8->V[chip8->inst.X] + chip8->V[chip8->inst.Y]) > 255) ;
                    chip8->V[chip8->inst.X] += chip8->V[chip8->inst.Y] ;
                    chip8->V[0xF] = carry ;
                    break;

                case 0x05 : 
                // 0x8XY5: Set register VX -= VY, set vf to 0 if borrow, 1 if not
                    carry= (chip8->V[chip8->inst.Y] <= chip8->V[chip8->inst.X]); // if vy >= vx , no borrow

                    chip8->V[chip8->inst.X] -= chip8->V[chip8->inst.Y] ;
                    chip8->V[0xF] = carry ;
                    break;
                case 0x06 : 
                // shift right and store least significant bit in VF
                    carry = chip8->V[chip8->inst.X] & 0x1 ;
                    chip8->V[chip8->inst.X] = chip8->V[chip8->inst.X] >> 1 ;
                    chip8->V[0xF] = carry ;
                    break;

                case 0x07 : 
                // 0x8XY7: Set register VX = VY - VX, set VF to 0 if borrow, 1 if not
                    carry = (chip8->V[chip8->inst.X] <= chip8->V[chip8->inst.Y]); // if vy >= vx , no borrow 

                    chip8->V[chip8->inst.X] = chip8->V[chip8->inst.Y] - chip8->V[chip8->inst.X] ;
                    chip8->V[0xF] = carry ;
                    break;

                case 0x0E : 
                     // 0x8XYE: Set register VX <<= 1, store shifted off bit in VF
                     // E 12: 8XYE verify that VF is set to the MSB (most significant bit or most left) before the shift and  VF does not take value 0 every time
                    carry = (chip8->V[chip8->inst.X] & 0x80) >> 7 ;// if the most significant bit of VY is 1, set carry to 1, else 0
                    chip8->V[chip8->inst.X] = chip8->V[chip8->inst.X] << 1 ;
                    chip8->V[0xF] = carry ;
                    break;

            default:
                break;
            }
            break;

        case 0x09 : 
            // 0x9XY0: Skip next instruction if VX != VY
            if ( chip8->V[chip8->inst.X] != chip8->V[chip8->inst.Y]) { 
                chip8->pc += 2 ;
                break; 
            }
            break;

        case 0x0A : 
            // 0xANNN: Set index register I to the address NNN
            chip8->I = chip8->inst.NNN ;
            break;

        case 0x0B : 
            // 0xBNNN: Jump to address NNN + V0
            chip8->pc = chip8->V[0] + chip8->inst.NNN ; 
            break; 
        case 0x0C : 
            // 0xCXNN: Set VX to random byte AND NN
            chip8->V[chip8->inst.X] = (rand() % 256) & chip8->inst.NN ; 
            break;
        
        case 0x0D : 
            // 0xDXYN: Draw sprite at coordinate (VX, VY) with width 8 pixels and height N pixels
            uint8_t VX = chip8->V[chip8->inst.X] % CHIP8_DISPLAY_WIDTH ; // wrap around if going off screen
            uint8_t VY = chip8->V[chip8->inst.Y] % CHIP8_DISPLAY_HEIGHT ; // wrap around if going off screen
            chip8->V[0xF] = 0 ; // reset collision flag
            uint8_t height = chip8->inst.N ;

            for ( int row = 0 ; row < height ; row ++) { 
                uint8_t sprite_byte = chip8->memory[chip8->I + row ] ; 
                for ( int col = 0 ; col < 8 ; col++) { 
                    if ( sprite_byte & (0x80 >> col)) // check if the current bit is set
                    { 
                        uint16_t pixel_index = ( (VY + row) % CHIP8_DISPLAY_HEIGHT) * CHIP8_DISPLAY_WIDTH + ( (VX + col) % CHIP8_DISPLAY_WIDTH) ; 
                        if ( chip8->display[pixel_index]) { 
                            chip8->V[0xF] = 1 ; // set collision flag
                        }
                        chip8->display[pixel_index] ^= 1 ; // XOR the pixel
                        
                    }
                }
            }
            break;

        case 0x0E :
            if(chip8->inst.NN == 0x9E) { 
                // key pressing 
                if (chip8->keypad[chip8->V[chip8->inst.X]]) { 
                    chip8->pc += 2 ;
                }
                break;
            }
            else if ( chip8->inst.NN == 0xA1) { 
                // TODO : key pressing 
                if (!chip8->keypad[chip8->V[chip8->inst.X]]) { 
                    chip8->pc += 2 ;
                    break;
                }
                break;

            }
            break;  
        case 0x0F :
            switch (chip8->inst.NN)
            {
            case 0x07:
                // 0xFX07: Set VX to the value of the delay timer
                chip8->V[chip8->inst.X] = chip8->delay_timer ;
                break;
            case 0x0A : 
                // 0xFX0A: Wait for a key press, store the value of the key in VX
                bool key_pressed = false;
                for ( uint8_t i = 0 ; i < sizeof(chip8->keypad) ; i++ ) { 
                    if ( chip8->keypad[i]) { 
                        chip8->V[chip8->inst.X] = i ; 
                        key_pressed = true ;
                        break ; 
                    }
                }
                if (!key_pressed) {
                    chip8->pc -= 2 ; // repeat this instruction
                }
                break ; 
            case 0x15 : // timer 
                // 0xFX15: Set the delay timer to VX
                chip8->delay_timer = chip8->V[chip8->inst.X ] ;
                break;
            case 0x18 : // sound 
                // 0xFX18: Set the sound timer to VX
                chip8->sound_timer = chip8->V[chip8->inst.X ] ;
                break;
            case 0x1E : // adds vx to I 
                chip8->I += chip8->V[chip8->inst.X ];
                break;
            case 0x29 :
                // set I to the location of the sprite for the character in VX 
                chip8->I = chip8->V[chip8->inst.X] * 5 ; // each sprite is 5 bytes long

                break; 
            case 0x33 :
                // store the binary-coded decimal representation of VX at I, I+1, and I+2
                chip8->memory[chip8->I] = chip8->V[chip8->inst.X] / 100 ;
                chip8->memory[chip8->I + 1] = (chip8->V[chip8->inst.X] /10) %10  ;
                chip8->memory[chip8->I + 2] = chip8->V[chip8->inst.X] % 10 ;
                break;
            case 0x55 :
                // store registers V0 to VX in memory starting at location I
                for (uint8_t i = 0 ; i <= chip8->inst.X ; i++ ) { 
                    chip8->memory[chip8->I + i] = chip8->V[i] ;
                }
                break;
            case 0x65 :
                // load registers V0 to VX from memory starting at location I
                for ( uint8_t i =0 ; i <= chip8->inst.X ; i++  ) {
                    chip8->V[i] = chip8->memory[chip8->I + i] ; 
                }

                
            default:
                break;
            }   
            break;
        default :   
            break;
    }

}