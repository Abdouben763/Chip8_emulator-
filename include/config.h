#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include <stdbool.h>



typedef struct {
    uint32_t window_width; // window dimensions
    uint32_t window_height; // window dimensions
    uint32_t scale_factor; // scaling factor for display
    uint32_t fg_color; // foreground color
    uint32_t bg_color; // background color
    bool pixelized; // whether to draw pixel borders
    uint32_t instructions_per_second; // Number of instructions to execute per second
    uint32_t sqr_freq; // Frequency in Hz
    int16_t volume; // Volume (0-128)
    uint32_t sample_rate; // Audio sample rate

} config_t;

bool init_config (config_t *config) ; 


#endif // CONFIG_H