#ifndef MAKISE_CONFIG_H
#define  MAKISE_CONFIG_H 1

//comment unused displays
#define MAKISE_SDL2_USE 1
//#define MAKISE_ILI9340_USE 1


#define MAKISEGUI_DRIVER_DEPTH 16
#define MAKISEGUI_BUFFER_DEPTH 16
#define MAKISEGUI_BUFFER_DEPTHMASK 0xFFFF


#define MAKISE_BUF_H 12
#define MAKISE_BUF_W 320

#define MAKISE_GUI_INPUT_BUFFER_LEN 10


#ifdef MAKISE_ILI9340_USE
#include "ili9340.h"
#endif

#endif
