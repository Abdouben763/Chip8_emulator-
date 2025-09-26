#include "input.h"




/*    CHIP-8 Keypad layout:  AZERTY Keyboard mapping:
        1 2 3 4        1 2 3 4
        Q W E R   =>   A Z E R
        A S D F        Q S D F
        Z X C V        W X C V

       */

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
                case SDLK_ESCAPE : 
                    chip8->state = STOPPED ; 
                    return ; 
                case SDLK_SPACE : 
                    if(chip8->state == RUNNING) { 
                        chip8->state = PAUSED ;
                        puts("=====PAUSED =======") ; 
                    } else { 
                        chip8->state = RUNNING  ; 
                    }
                    return ; 
                
                default : 
                    break; 

            }
        case SDL_KEYUP : 
            break;

        default : 
            break; 
        }
    }


}