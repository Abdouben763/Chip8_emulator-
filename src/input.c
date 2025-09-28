#include "input.h"

// CHIP-8 Input Handling
// Manages keyboard input, save/load states, and emulator controls

/*    CHIP-8 Keypad layout:  AZERTY Keyboard mapping:
        1 2 3 4        1 2 3 4
        Q W E R   =>   A Z E R
        A S D F        Q S D F
        Z X C V        W X C V

    For QWERTY keyboards, uncomment the mapping below:
     1 2 3 4 => 1 2 3 4
     Q W E R => Q W E R  
     A S D F => A S D F
     Z X C V => Z X C V
       */

// Handle all SDL events and keyboard input
void handle_input (chip8_t *chip8) {
    SDL_Event event ; 
    while (SDL_PollEvent(&event)) {
        switch (event.type)
        {
        case SDL_QUIT:
            chip8->state = STOPPED; 
            break;
        case SDL_KEYDOWN : 
            switch (event.key.keysym.sym) {
                // System controls
                case SDLK_ESCAPE : 
                    chip8->state = STOPPED ; 
                    return ; 
                case SDLK_SPACE : 
                    // Toggle pause/resume
                    if(chip8->state == RUNNING) { 
                        chip8->state = PAUSED ;
                        puts("=====PAUSED =======") ; 
                    } else { 
                        chip8->state = RUNNING  ; 
                        puts("=====RUNNING =======") ;
                    }
                    return ; 
                case SDLK_m : 
                    // Reset emulator
                    init_chip8(chip8 , chip8->rom_name ) ;
                    break;
                // Save states (F1-F4)
                case SDLK_F1 :
                    if (save_state(chip8 , chip8->save_filename , sizeof(chip8->save_filename) , 1))
                        puts ("State saved successfully in slot 1 !") ;
                    else
                        puts ("Failed to save state !") ;
                    break;

                case SDLK_F2 :
                    if (save_state(chip8 , chip8->save_filename , sizeof(chip8->save_filename) , 2))
                        puts ("State saved successfully in slot 2 !") ;
                    else
                        puts ("Failed to save state !") ;
                    break;

                case SDLK_F3 :
                    if (save_state(chip8 , chip8->save_filename , sizeof(chip8->save_filename) , 3))
                        puts ("State saved successfully in slot 3 !") ;
                    else
                        puts ("Failed to save state !") ;
                    break;
                case SDLK_F4 :
                    if (save_state(chip8 , chip8->save_filename , sizeof(chip8->save_filename) , 4))
                        puts ("State saved successfully in slot 4 !") ;
                    else
                        puts ("Failed to save state !") ;
                    break;  

                // Load states (F5-F8)
                case SDLK_F5 :
                    if (load_state(chip8 , chip8->save_filename , sizeof(chip8->save_filename) , 1))
                        puts ("State loaded successfully from slot 1 !") ;
                    else
                        puts ("Failed to load state !") ;
                    break;

                case SDLK_F6 :
                    if (load_state(chip8 , chip8->save_filename , sizeof(chip8->save_filename) , 2))
                        puts ("State loaded successfully from slot 2 !") ;
                    else
                        puts ("Failed to load state !") ;
                    break;

                case SDLK_F7 :
                    if (load_state(chip8 , chip8->save_filename , sizeof(chip8->save_filename) , 3))
                        puts ("State loaded successfully from slot 3 !") ;
                    else
                        puts ("Failed to load state !") ;
                    break;

                case SDLK_F8 :
                    if (load_state(chip8 , chip8->save_filename , sizeof(chip8->save_filename) , 4))
                        puts ("State loaded successfully from slot 4 !") ;
                    else
                        puts ("Failed to load state !") ;
                    break;
                   
                // CHIP-8 keypad mapping (AZERTY layout)
                case SDLK_1 : chip8->keypad[0x1] = true ; break;
                case SDLK_2 : chip8->keypad[0x2] = true ; break;
                case SDLK_3 : chip8->keypad[0x3] = true ; break;
                case SDLK_4 : chip8->keypad[0xC] = true ; break;
                case SDLK_a : chip8->keypad[0x4] = true ; break;
                case SDLK_z : chip8->keypad[0x5] = true ; break;
                case SDLK_e : chip8->keypad[0x6] = true ; break;
                case SDLK_r : chip8->keypad[0xD] = true ; break;
                case SDLK_q : chip8->keypad[0x7] = true ; break;
                case SDLK_s : chip8->keypad[0x8] = true ; break;
                case SDLK_d : chip8->keypad[0x9] = true ; break;
                case SDLK_f : chip8->keypad[0xE] = true ; break;
                case SDLK_w : chip8->keypad[0xA] = true ; break;
                case SDLK_x : chip8->keypad[0x0] = true ; break;
                case SDLK_c : chip8->keypad[0xB] = true ; break;
                case SDLK_v : chip8->keypad[0xF] = true ; break;
                // CHIP-8 keypad mapping (QWERTY layout)
                /*
                case SDLK_1 : chip8->keypad[0x1] = true ; break;
                case SDLK_2 : chip8->keypad[0x2] = true ; break;
                case SDLK_3 : chip8->keypad[0x3] = true ; break;
                case SDLK_4 : chip8->keypad[0xC] = true ; break;
                case SDLK_q : chip8->keypad[0x4] = true ; break;
                case SDLK_w : chip8->keypad[0x5] = true ; break;
                case SDLK_e : chip8->keypad[0x6] = true ; break;
                case SDLK_r : chip8->keypad[0xD] = true ; break;
                case SDLK_a : chip8->keypad[0x7] = true ; break;
                case SDLK_s : chip8->keypad[0x8] = true ; break;
                case SDLK_d : chip8->keypad[0x9] = true ; break;
                case SDLK_f : chip8->keypad[0xE] = true ; break;
                case SDLK_z : chip8->keypad[0xA] = true ; break;
                case SDLK_x : chip8->keypad[0x0] = true ; break;
                case SDLK_c : chip8->keypad[0xB] = true ; break;
                case SDLK_v : chip8->keypad[0xF] = true ; break; 
                */
                
                default : 
                    break; 

            }
            break;
        case SDL_KEYUP : 
            // Release CHIP-8 keypad buttons
            switch (event.key.keysym.sym) {
                case SDLK_1 : chip8->keypad[0x1] = false ; break;
                case SDLK_2 : chip8->keypad[0x2] = false ; break;
                case SDLK_3 : chip8->keypad[0x3] = false ; break;
                case SDLK_4 : chip8->keypad[0xC] = false ; break;
                case SDLK_a : chip8->keypad[0x4] = false ; break;
                case SDLK_z : chip8->keypad[0x5] = false ; break;
                case SDLK_e : chip8->keypad[0x6] = false ; break;
                case SDLK_r : chip8->keypad[0xD] = false ; break;
                case SDLK_q : chip8->keypad[0x7] = false ; break;
                case SDLK_s : chip8->keypad[0x8] = false ; break;
                case SDLK_d : chip8->keypad[0x9] = false ; break;
                case SDLK_f : chip8->keypad[0xE] = false ; break;
                case SDLK_w : chip8->keypad[0xA] = false ; break;
                case SDLK_x : chip8->keypad[0x0] = false ; break;
                case SDLK_c : chip8->keypad[0xB] = false ; break;
                case SDLK_v : chip8->keypad[0xF] = false ; break;
                // CHIP-8 keypad mapping (QWERTY layout)
                /*
                case SDLK_1 : chip8->keypad[0x1] = false ; break;
                case SDLK_2 : chip8->keypad[0x2] = false ; break;
                case SDLK_3 : chip8->keypad[0x3] = false ; break;
                case SDLK_4 : chip8->keypad[0xC] = false ; break;
                case SDLK_q : chip8->keypad[0x4] = false ; break;
                case SDLK_w : chip8->keypad[0x5] = false ; break;
                case SDLK_e : chip8->keypad[0x6] = false ; break;
                case SDLK_r : chip8->keypad[0xD] = false ; break;
                case SDLK_a : chip8->keypad[0x7] = false ; break;
                case SDLK_s : chip8->keypad[0x8] = false ; break;
                case SDLK_d : chip8->keypad[0x9] = false ; break;
                case SDLK_f : chip8->keypad[0xE] = false ; break;
                case SDLK_z : chip8->keypad[0xA] = false ; break;
                case SDLK_x : chip8->keypad[0x0] = false ; break;
                case SDLK_c : chip8->keypad[0xB] = false ; break;
                case SDLK_v : chip8->keypad[0xF] = false ; break; 
                */  

                default : 
                    break; 
            }
            break;

        default : 
            break; 
        }

    }


}