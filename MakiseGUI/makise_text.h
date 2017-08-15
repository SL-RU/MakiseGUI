#ifndef MAKISE_TEXT_H
#define MAKISE_TEXT_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include "makise.h"
#include "makise_config.h"    

#define MDTextAll UINT32_MAX 

typedef enum {
    MDTextPlacement_LeftUp = 0,
    MDTextPlacement_Center,
    MDTextPlacement_CenterUp,
    MDTextPlacement_CenterDown,
} MDTextPlacement;


typedef struct _MakiseFont {
    const uint8_t*      table; //draw data
    uint16_t            width; //common width
    uint16_t            height; //common height
    const uint8_t*      char_width; //width of each char
    const uint16_t*     char_index; //start index of char in the table
#if MAKISE_UNICODE
    const uint16_t*     unicode_index; //indexes of unicode characters with id>127. Format is array's id -> unicode code. Where array's id is index of char in the char_index & char_width
#endif
    //first block without gasps
    uint16_t            offset; //char's code
    uint16_t            num_char; //count chars in block
#if MAKISE_UNICODE
    uint16_t            num_uni; //total count
#endif
    uint16_t            space_char;
} MakiseFont;


// Draw single char.
void        makise_d_char          ( MakiseBuffer *b,
                                     uint16_t ch,
                                     int16_t x, int16_t y,
                                     const MakiseFont* font,
                                     uint32_t c );

// Draw string.
void        makise_d_string        ( MakiseBuffer *b,
                                     char *s,
                                     uint32_t len,
                                     int16_t x, int16_t y,
                                     MDTextPlacement place,
                                     const MakiseFont* font,
                                     uint32_t c);

// Get width of string being drew using that font.
uint32_t    makise_d_string_width  ( char* s,
                                     uint32_t len,
                                     const MakiseFont* font );

// Draw multiline text in the defined frame.
void        makise_d_string_frame  ( MakiseBuffer *b,
                                     char *s,
                                     uint32_t len,
                                     int16_t x, int16_t y,
                                     uint16_t w, uint16_t h,
                                     const MakiseFont *font,
                                     uint16_t line_spacing,
                                     uint32_t c );

// Get height text for user width window.
uint32_t makise_d_string_height_get ( char*             s,
                                      uint32_t          len,
                                      uint16_t          width_window,
                                      const MakiseFont* font,
                                      uint32_t          font_line_spacing );


#if MAKISE_UNICODE    
/**
 * Get index of utf-8 character
 *
 * @param s character's bytes
 * @param len maximum available count of bytes
 * @param bts bytes per char
 * @return 0 if is error. Else - unicode index of char
 */
    uint32_t    makise_d_utf_char_id  ( char *s, uint32_t len, uint8_t *bts );
    
/**
 * Get index of utf-8 character in the font table
 *
 * @param c utf-8 index
 * @param font font
 * @return -1 if is error. Else - index of char in the table
 */
    uint32_t    makise_d_utf_char_font  ( uint32_t c, const MakiseFont *font);
#endif //unicode

#ifdef __cplusplus
}
#endif //cpp

#endif //MAKISE_TEXT_H
