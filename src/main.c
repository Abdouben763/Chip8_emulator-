#include "display.h"
#include "chip8.h"
#include "input.h"

int main(int argc, char const *argv[]) {
    // usage
    if (argc < 2 ) {
        fprintf ( stderr , "Usage %s <rom_name>\n" , argv[1] ) ;
        exit(EXIT_FAILURE) ;
    }
    // Initialize SDL
    sdl_t sdl = {0};
    if (!init_display(&sdl)) exit(EXIT_FAILURE); 

    chip8_t chip8 = {0} ; 
    
    const char *rom_name = argv[1] ; 
    if(!init_chip8(&chip8 , rom_name)) exit(EXIT_FAILURE) ; 
    
    // clear display 
    clear_display(&sdl) ;

    // main loop 
    while (chip8.state != STOPPED)
    {
        handle_input(&chip8) ; 
        if (chip8.state == PAUSED) continue ;
        run_intructions(&chip8) ;
        SDL_Delay(16);  // ~60Hz
        update_display(&sdl , &chip8 ) ;
    }
    
    clear_display(&sdl) ;
    exit(EXIT_SUCCESS) ;
    

}
