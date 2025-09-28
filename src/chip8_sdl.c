/** 
 * @file display.c
 * @brief SDL2 Display and Audio Handling for CHIP-8 Emulator
 * @author Abderrahmane Benchikh
 * @date 2025
 * 
 * This file implements the display and audio functionalities using SDL2.
 * It includes initialization, rendering the CHIP-8 display, clearing the
 * screen, and audio callback for sound generation.
 */
#include "chip8_sdl.h"


void audio_callback(void *userdata, uint8_t *stream, int len) {
    config_t *config = (config_t *)userdata;
    int16_t *data = (int16_t *)stream;
    static uint32_t sample_counter = 0 ; 
    const int32_t sqr_wave_T = config->sample_rate / config->sqr_freq;
    int32_t half_sqr_wave_T = sqr_wave_T / 2;
   

    for (int i = 0; i < len / 2; i++) {
        if ( (sample_counter / half_sqr_wave_T) %2 ) {
            data[i] = config->volume ; // High part of the square wave
        } else {
            data[i] = -config->volume ; // Low part of the square wave
        }
        sample_counter++ ;
    }
}

bool init_display( sdl_t * sdl , config_t *config ) { 
    // Initialize SDL
    if (SDL_Init ( SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER ) !=0) { 
        SDL_Log ( "Unable to initialize SDL: %s\n", SDL_GetError() ) ;
        return false; 
    }
    // Create window and renderer
    sdl->window = SDL_CreateWindow ( "CHIP-8 Emulator" , SDL_WINDOWPOS_CENTERED ,SDL_WINDOWPOS_CENTERED ,config->window_width, config->window_height, 0 ) ;

    if ( !sdl->window ) {
        SDL_Log ( "Could not create window: %s\n", SDL_GetError() ) ;  
        return false ; 
    }
    sdl->renderer = SDL_CreateRenderer ( sdl->window , -1 , SDL_RENDERER_ACCELERATED ) ;
    if ( !sdl->renderer ) {
        SDL_Log ( "Could not create renderer: %s\n", SDL_GetError() ) ;
        return false ;
    }
    // Initialize audio
    sdl->desired_spec = (SDL_AudioSpec) {
        .freq = 44100 ,
        .format = AUDIO_S16LSB ,
        .channels = 1 ,
        .samples = 512 ,
        .callback = audio_callback,
        // user data from the config.h file
        .userdata = config,  // config is already a pointer, don't take address again
    };
    sdl->chip8_audio_device = SDL_OpenAudioDevice (NULL, 0 , &sdl->desired_spec , &sdl->obtained_spec , 0 ) ;

    // Error handling 
    if ( sdl->chip8_audio_device == 0 ) { 
        SDL_Log ( "Failed to open audio: %s\n" , SDL_GetError() ) ; 
        return false ; 
    }
    if (sdl->desired_spec.format != sdl->obtained_spec.format ||
        sdl->desired_spec.channels != sdl->obtained_spec.channels ||
        sdl->desired_spec.freq != sdl->obtained_spec.freq ||
        sdl->desired_spec.samples != sdl->obtained_spec.samples ) {
        SDL_Log ( "Failed to get the desired AudioSpec\n" ) ;
        return false ; 
    }


    return true ; // success
}

void close_display ( sdl_t * sdl ) { 
    SDL_DestroyRenderer ( sdl->renderer ) ; 
    SDL_DestroyWindow ( sdl->window ) ; 
    SDL_CloseAudioDevice ( sdl->chip8_audio_device ) ;
    SDL_Quit() ;
}

void clear_display ( sdl_t *sdl , config_t config ) {
    // for example : bg_color = 0x000000FF
    uint8_t r = (config.bg_color >> 24) & 0xFF ; // red = 0x00 
    uint8_t g = (config.bg_color >> 16) & 0xFF ; // green = 0x00
    uint8_t b = (config.bg_color >> 8) & 0xFF ; // blue = 0x00
    uint8_t a = config.bg_color & 0xFF ; // alpha = 0xFF 

    SDL_SetRenderDrawColor ( sdl->renderer , r , g , b , a ) ;
    SDL_RenderClear ( sdl->renderer ) ;
}

void update_display ( sdl_t *sdl , chip8_t *chip8 , config_t config ) {
    SDL_Rect rect = {.x=0, .y=0, .w=config.scale_factor, .h=config.scale_factor} ;


    // grab foreground color
    uint8_t fg_r = (config.fg_color >> 24) & 0xFF ; // red = 0xFF
    uint8_t fg_g = (config.fg_color >> 16) & 0xFF ; // green = 0xFF
    uint8_t fg_b = (config.fg_color >> 8) & 0xFF ; // blue = 0xFF
    uint8_t fg_a = config.fg_color & 0xFF ; // alpha = 0xFF

    // Grab background color
    uint8_t bg_r = (config.bg_color >> 24) & 0xFF ; // red = 0x00
    uint8_t bg_g = (config.bg_color >> 16) & 0xFF ; // green = 0x00
    uint8_t bg_b = (config.bg_color >> 8) & 0xFF ; // blue = 0x00
    uint8_t bg_a = config.bg_color & 0xFF ; // alpha = 0xFF

    for (uint32_t i = 0 ; i< sizeof (chip8->display) ; i++ ) { 
        rect.x = (i % CHIP8_DISPLAY_WIDTH) * config.scale_factor ;
        rect.y = (i / CHIP8_DISPLAY_WIDTH) * config.scale_factor ;

        if ( chip8->display[i]) { 
            SDL_SetRenderDrawColor ( sdl->renderer , fg_r , fg_g , fg_b , fg_a ) ;
            SDL_RenderFillRect ( sdl->renderer , &rect ) ;

            if (config.pixelized) { 
                SDL_SetRenderDrawColor ( sdl->renderer , bg_r , bg_g , bg_b , bg_a ) ;
                SDL_RenderDrawRect ( sdl->renderer , &rect ) ;
            }
        } else { 
            SDL_SetRenderDrawColor ( sdl->renderer , bg_r , bg_g , bg_b , bg_a ) ;
            SDL_RenderFillRect ( sdl->renderer , &rect ) ;
        }
    }
    SDL_RenderPresent ( sdl->renderer ) ;

} 