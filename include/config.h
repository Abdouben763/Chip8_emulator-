#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include <stdbool.h>



typedef struct {
    uint32_t window_width;
    uint32_t window_height;
    uint32_t scale_factor;
    uint32_t fg_color;
    uint32_t bg_color;
    bool pixelized;
    uint32_t instructions_per_second;
    // audio settings
    uint32_t sqr_freq; // Frequency in Hz
    int16_t volume; // Volume (0-128)
    uint32_t sample_rate; // Audio sample rate

} config_t;

bool init_config (config_t *config) ; 


#endif // CONFIG_H