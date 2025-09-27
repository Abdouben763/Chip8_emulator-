#include "display.h"
#include "chip8.h"
#include "input.h"
#include "timer.h"


int main(int argc, char const *argv[]) {
    // usage
    if (argc < 2 ) {
        fprintf ( stderr , "Usage %s <rom_name>\n" , argv[1] ) ;
        exit(EXIT_FAILURE) ;
    }
        // config settings
    config_t config = {0} ; 
    if (!init_config(&config)) exit(EXIT_FAILURE) ;

    // Initialize SDL
    sdl_t sdl = {0};
    if (!init_display(&sdl , &config)) exit(EXIT_FAILURE); 

    chip8_t chip8 = {0} ; 
    const char *rom_name = argv[1] ; 
    if(!init_chip8(&chip8 , rom_name)) exit(EXIT_FAILURE) ; 


    // clear display
    clear_display(&sdl , config) ;

    // main loop 
    while (chip8.state != STOPPED)
    {
        handle_input(&chip8) ; 
        if (chip8.state == PAUSED) continue ;

        // Get time before executing instructions
        uint32_t start_time = SDL_GetPerformanceCounter ();
        for( uint32_t i = 0 ; i < config.instructions_per_second / 60 ; i++ ) {
            run_intructions(&chip8) ;
        }
        // Get time after executing instructions
        uint32_t end_time = SDL_GetPerformanceCounter () ;
        // Calculate elapsed time in milliseconds
        double elapsed_time = (double) ( end_time - start_time) *1000 / SDL_GetPerformanceFrequency() ;
        uint32_t delay ; 
        if (elapsed_time < 16.67) {
            delay = 16.67 - elapsed_time ;
        } else {
            delay = 0 ; 
        }

        SDL_Delay(delay);  // ~60Hz
        update_display(&sdl , &chip8 , config ) ;
        update_timers(&sdl , &chip8 ) ;
    }

    clear_display(&sdl , config) ;
    exit(EXIT_SUCCESS) ;
    

}
