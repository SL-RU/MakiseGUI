#ifndef MAKISE_TEXT_H
#define MAKISE_TEXT_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include "makise.h"


#define MDTextAll UINT32_MAX 

typedef enum
{
    MDTextPlacement_LeftUp = 0,
    MDTextPlacement_Center,
    MDTextPlacement_CenterUp,
    MDTextPlacement_CenterDown,
} MDTextPlacement;


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

#include "fonts.h"

void makise_d_char(MakiseBuffer *b,
		   uint16_t ch,
		   int16_t x, int16_t y,
		   const MakiseFont *font, uint32_t c); //draw single char
void makise_d_string(MakiseBuffer *b,
		     char *s, uint32_t len,
		     int16_t x, int16_t y, MDTextPlacement place,
		     const MakiseFont *font, uint32_t c); //draw string
uint32_t makise_d_string_width(char *s, uint32_t len, const MakiseFont *font); //get width of string being drew using that font
void makise_d_string_frame(MakiseBuffer *b,
			   char *s, uint32_t len,
			   int16_t x, int16_t y, uint16_t w, uint16_t h,
			   const MakiseFont *font, uint16_t line_spacing, uint32_t c); //draw multiline text in the defined frame

#ifdef __cplusplus
}
#endif

#endif
