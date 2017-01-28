#include "makise_colors.h"


#if MAKISEGUI_BUFFER_DEPTH != MAKISEGUI_DRIVER_DEPTH
#if MAKISEGUI_DRIVER_DEPTH == 16
uint32_t _makise_color_palette_default[] = {
    //BRG 565
    0x0000, //Black   
    0xffff, //White   
    0x07c0, //Red     
    0x001f, //Green   
    0xf800, //Blue    
    0x07ff, //Yellow    
    0xffc0, //Magenta 
    0xf83f, //Cyan    
    0x003f, //Lime    
    0b0000100001000010, //Gray    
    0x02c0, //Maroon  
    0x5800, //Navy
    0x781f, //Teal     
    0x03df, //Olive    
    0x0017, //DarkGreen
    0x1400, //Brawn        
};
#endif

uint32_t *makise_color_palette = _makise_color_palette_default;
#endif

inline uint32_t makise_color_get(uint32_t c)
{
    #if MAKISEGUI_BUFFER_DEPTH == MAKISEGUI_DRIVER_DEPTH

    return c;

    #elif MAKISEGUI_BUFFER_DEPTH != MAKISEGUI_DRIVER_DEPTH

    return makise_color_palette[c];
    
    #endif
}
