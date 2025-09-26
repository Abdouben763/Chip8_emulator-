#include "timer.h"

void update_timers ( chip8_t *chip8 ) { 
    if ( chip8->delay_timer > 0 ) { 
        chip8->delay_timer -- ; 
    }
    if ( chip8->sound_timer > 0 ) { 
        chip8->sound_timer -- ; 
        
    }
}