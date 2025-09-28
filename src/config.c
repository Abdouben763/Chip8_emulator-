#include "config.h"


// Configuration settings


bool init_config (config_t *config) {
    config->window_width = 640;
    config->window_height = 320;
    config->scale_factor = 10;
    config->fg_color = 0xFFFFFFFF;
    config->bg_color = 0x000000FF;
    config->pixelized = true;
    config->instructions_per_second = 500;
    config->sqr_freq = 440; // Frequency in Hz
    config->volume = 3000; // Volume (0-128)
    config->sample_rate = 44100; // Samples per second
    return true; // success
}

