#include "makise.h"
#include "makise_config.h"
#if MAKISE_SDL2_USE == 1
#ifndef MAKISE_SDL2_H
#define MAKISE_SDL2_H

#include <stdint.h>
#include <SDL2/SDL.h>

void makise_sdl2_driver(MakiseDriver* d, uint32_t width, uint32_t height, SDL_Surface *s);

void makise_sdl2_draw(MakiseGUI* gui);

#endif
#endif
