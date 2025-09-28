/**
 * @file main.c
 * @brief CHIP-8 Emulator Main Entry Point
 * @author Abderrahmane Benchikh
 * @date 2025
 * 
 * Main program that initializes all emulator systems and runs the main 
 * emulation loop at 60 FPS. Handles ROM loading, timing, input processing,
 * and coordinates between all emulator modules.
 */

#include "chip8_sdl.h"
#include "chip8.h"
#include "input.h"
#include "timer.h"
#include "config.h"


int main(int argc, char const *argv[]) {
    // Validate command line arguments
    if (argc < 2 ) {
        fprintf ( stderr , "Usage %s <rom_name>\n" , argv[1] ) ;
        exit(EXIT_FAILURE) ;
    }
    
    // Initialize configuration settings
    config_t config = {0} ; 
    if (!init_config(&config)) exit(EXIT_FAILURE) ;

    // Initialize SDL (graphics, audio, input)
    sdl_t sdl = {0};
    if (!init_display(&sdl , &config)) exit(EXIT_FAILURE); 
    
    // Initialize CHIP-8 system and load ROM
    chip8_t chip8 = {0} ; 
    const char *rom_name = argv[1] ; 
    if(!init_chip8(&chip8 , rom_name)) exit(EXIT_FAILURE) ; 


    // Clear screen and show controls
    clear_display(&sdl , config) ;
    puts("Press Space to pause/resume, M to reset, ESC to quit") ;
    
    // Main emulation loop - runs at 60 FPS
    while (chip8.state != STOPPED)
    {
        // Handle user input and system events
        handle_input(&chip8) ; 
        if (chip8.state == PAUSED) continue ;

        // Execute CHIP-8 instructions for this frame
        uint32_t start_time = SDL_GetPerformanceCounter ();
        // Run multiple instructions per frame based on config
        for( uint32_t i = 0 ; i < config.instructions_per_second / 60 ; i++ ) {
            run_intructions(&chip8) ;
        }
        
        // Calculate frame timing to maintain 60 FPS
        uint32_t end_time = SDL_GetPerformanceCounter () ;
        double elapsed_time = (double) ( end_time - start_time) *1000 / SDL_GetPerformanceFrequency() ;
        uint32_t delay ; 
        if (elapsed_time < 16.67) {
            delay = 16.67 - elapsed_time ;  // Target: 16.67ms per frame (60 FPS)
        } else {
            delay = 0 ;  // Frame took too long, no delay
        }

        SDL_Delay(delay);  // Maintain consistent frame rate
        update_display(&sdl , &chip8 , config ) ;  // Render graphics
        update_timers(&sdl , &chip8 ) ;  // Update delay and sound timers
    }

    // Cleanup and exit
    clear_display(&sdl , config) ;
    exit(EXIT_SUCCESS) ;
}
