#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "chip8.h"
#include "config.h"


typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_AudioDeviceID audio_device; 
    SDL_AudioSpec audio_spec;

} sdl_t;

bool init_display ( sdl_t * sdl  ) ; 
void close_display ( sdl_t * sdl ) ; 
void clear_display ( sdl_t *sdl ) ; 
void update_display ( sdl_t *sdl , chip8_t *chip8 ) ; 


#endif // DISPLAY_H