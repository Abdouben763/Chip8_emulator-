#include "timer.h"

void update_timers ( sdl_t *sdl , chip8_t *chip8 ) { 
    if ( chip8->delay_timer > 0 ) { 
        chip8->delay_timer -- ; 
    }
    if ( chip8->sound_timer > 0 ) { 
        chip8->sound_timer -- ; 
        SDL_PauseAudioDevice ( sdl->chip8_audio_device , 0 ) ;// Play audio

    } else { 
        SDL_PauseAudioDevice ( sdl->chip8_audio_device , 1 ) ; // Pause audio

    }
}