#ifndef MAKISE_COLORS_H 
#define MAKISE_COLORS_H 1
#include "makise_config.h"

#define MC_Transparent UINT16_MAX

#if defined MAKISEGUI_BUFFER_DEPTH && MAKISEGUI_BUFFER_DEPTH <= 4
typedef enum
{
    MC_Black        = 0,
    MC_White           ,
#if MAKISEGUI_BUFFER_DEPTH >= 2
    MC_Red             ,
    MC_Green           ,
#endif
#if MAKISEGUI_BUFFER_DEPTH == 4
    MC_Blue            ,
    MC_Yellow          ,
    MC_Magenta         ,
    MC_Cyan            ,
    MC_Lime            ,
    MC_Gray            ,
    MC_Maroon          ,
    MC_Navy            ,
#endif
} MColor;
#endif

#if MAKISEGUI_BUFFER_DEPTH == 16 && MAKISEGUI_BUFFER_DEPTH == MAKISEGUI_DRIVER_DEPTH
#define	MC_Black   0x0000
#define MC_White   0xFFFF
#define	MC_Red     0xF800
#define	MC_Green   0x07E0
#define	MC_Blue    0x001F
#define MC_Yellow  0xFFE0  
#define MC_Magenta 0xF81F
#define MC_Cyan    0x07FF
#define MC_Lime    0x7FFA
#define MC_Gray    0x94B2
#define MC_Maroon  0x8000
#define MC_Navy    0x0010
#endif

#if MAKISEGUI_BUFFER_DEPTH != MAKISEGUI_DRIVER_DEPTH
uint32_t *makise_color_palette;
#endif

//Get color value from value from buffer display.
uint32_t makise_color_get(uint32_t c);

#endif
