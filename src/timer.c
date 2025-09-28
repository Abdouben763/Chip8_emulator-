#include "timer.h"

// CHIP-8 Timer Management
// Handles delay and sound timers that decrement at 60Hz

// Update delay and sound timers at ~60Hz
void update_timers ( sdl_t *sdl , chip8_t *chip8 ) { 
    // Delay timer: used for timing events in games
    if ( chip8->delay_timer > 0 ) { 
        chip8->delay_timer -- ; 
    }
    
    // Sound timer: controls audio playback
    if ( chip8->sound_timer > 0 ) { 
        chip8->sound_timer -- ; 
        SDL_PauseAudioDevice ( sdl->chip8_audio_device , 0 ) ; // Play beep sound
    } else { 
        SDL_PauseAudioDevice ( sdl->chip8_audio_device , 1 ) ; // Stop sound
    }
}