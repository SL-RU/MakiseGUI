#ifndef MAKISE_STYLE_H
#define MAKISE_STYLE_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include "makise_gui.h"

typedef struct _MakiseStyleTheme
{
    MColor bg_color;
    MColor font_col;
    MColor border_c;

    uint16_t thickness;
} MakiseStyleTheme;

typedef struct _MakiseStyle {
    uint32_t maincolor;
    const MakiseFont * font;
    uint32_t font_line_spacing;

    MakiseStyleTheme unactive;
    MakiseStyleTheme normal;
    MakiseStyleTheme focused;
    MakiseStyleTheme active;
    
} MakiseStyle;

#ifdef __cplusplus
}
#endif

#endif
