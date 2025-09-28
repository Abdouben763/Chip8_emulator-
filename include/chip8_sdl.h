#ifndef CHIP8_SDL_H
#define CHIP8_SDL_H

#include <stdint.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "chip8.h"
#include "config.h"


typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_AudioDeviceID chip8_audio_device; 
    SDL_AudioSpec desired_spec , obtained_spec ;
} sdl_t;

bool init_display ( sdl_t * sdl , config_t *config ) ; 
void close_display ( sdl_t * sdl ) ; 
void clear_display ( sdl_t *sdl , config_t config ) ; 
void update_display ( sdl_t *sdl , chip8_t *chip8 , config_t config ) ;
void audio_callback ( void *userdata , uint8_t * stream , int len ) ;


#endif // chip8_SDL_H