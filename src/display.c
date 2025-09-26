
#include "display.h"

bool init_display( sdl_t * sdl ) { 
    // Initialize SDL
    if (SDL_Init ( SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER ) !=0) { 
        SDL_Log ( "Unable to initialize SDL: %s", SDL_GetError() ) ;
        return false; 
    }
    // Create window and renderer
    sdl->window = SDL_CreateWindow ( "CHIP-8 Emulator" , SDL_WINDOWPOS_CENTERED ,SDL_WINDOWPOS_CENTERED ,window_width, window_height, 0 ) ; 

    if ( !sdl->window ) {
        fprintf ( stderr, "Could not create window: %s\n", SDL_GetError() ) ;  
        return false ; 
    }
    sdl->renderer = SDL_CreateRenderer ( sdl->window , -1 , SDL_RENDERER_ACCELERATED ) ;
    if ( !sdl->renderer ) {
        fprintf ( stderr, "Could not create renderer: %s\n", SDL_GetError() ) ; 
        return false ;
    }

    return true ; // succes 
}

void close_display ( sdl_t * sdl ) { 
    SDL_DestroyRenderer ( sdl->renderer ) ; 
    SDL_DestroyWindow ( sdl->window ) ; 
    SDL_Quit() ;
}

void clear_display ( sdl_t *sdl ) {
    // for example : bg_color = 0x000000FF
    uint8_t r = (BG_color >> 24) & 0xFF ; // red = 0x00 
    uint8_t g = (BG_color >> 16) & 0xFF ; // green = 0x00
    uint8_t b = (BG_color >> 8) & 0xFF ; // blue = 0x00
    uint8_t a = BG_color & 0xFF ; // alpha = 0xFF 

    SDL_SetRenderDrawColor ( sdl->renderer , r , g , b , a ) ;
    SDL_RenderClear ( sdl->renderer ) ;
}

void update_display ( sdl_t *sdl , chip8_t *chip8 ) {
    SDL_Rect rect = {.x=0, .y=0, .w=scale_factor, .h=scale_factor} ;


    // grab foreground color
    uint8_t fg_r = (FG_color >> 24) & 0xFF ; // red = 0xFF
    uint8_t fg_g = (FG_color >> 16) & 0xFF ; // green = 0xFF
    uint8_t fg_b = (FG_color >> 8) & 0xFF ; // blue = 0xFF
    uint8_t fg_a = FG_color & 0xFF ; // alpha = 0xFF    
    
    // Grab background color 
    uint8_t bg_r = (BG_color >> 24) & 0xFF ; // red = 0x00
    uint8_t bg_g = (BG_color >> 16) & 0xFF ; // green = 0x00
    uint8_t bg_b = (BG_color >> 8) & 0xFF ; // blue = 0x00
    uint8_t bg_a = BG_color & 0xFF ; // alpha = 0xFF

    for (uint32_t i = 0 ; i< sizeof (chip8->display) ; i++ ) { 
        rect.x = (i % CHIP8_DISPLAY_WIDTH) * scale_factor ;
        rect.y = (i / CHIP8_DISPLAY_WIDTH) * scale_factor ;

        if ( chip8->display[i]) { 
            SDL_SetRenderDrawColor ( sdl->renderer , fg_r , fg_g , fg_b , fg_a ) ;
            SDL_RenderFillRect ( sdl->renderer , &rect ) ;

            if (pixelized) { 
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