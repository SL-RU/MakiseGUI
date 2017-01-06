#ifndef MAKISE_TEXT_H
#define MAKISE_TEXT_H 1

#include <string.h>
#include "makise.h"


typedef struct _MakiseFont MakiseFont;

#include "fonts.h"

typedef struct _MakiseFont
{
    const uint8_t * table;
    uint16_t width;
    uint16_t height;
    const uint8_t * char_width;
    const uint16_t * char_index;
    uint16_t offset;
    uint16_t num_char;
    uint16_t space_char;    
} MakiseFont;


void makise_d_char(MakiseBuffer *b, uint16_t ch, uint16_t x, uint16_t y, const MakiseFont *font, uint32_t c);
void makise_d_string(MakiseBuffer *b, char *s, uint16_t x, uint16_t y, const MakiseFont *font, uint32_t c);

#endif
