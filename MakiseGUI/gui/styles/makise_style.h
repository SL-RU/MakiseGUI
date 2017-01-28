#include "makise_gui.h"

typedef struct _MakiseStyleTheme
{
    uint32_t bg_color;
    uint32_t font_col;
    uint32_t border_c;

    uint16_t double_border;
} MakiseStyleTheme;

typedef struct _MakiseStyle {
    char* name;
    
    uint32_t maincolor;

    MakiseStyleTheme unactive;
    MakiseStyleTheme normal;
    MakiseStyleTheme focused;
    MakiseStyleTheme active;
    
} MakiseStyle;
