#ifndef MAKISE_CONFIG_H
#define  MAKISE_CONFIG_H 1

//comment unused displays
#define MAKISE_ILI9340_USE 1


#define MAKISEGUI_DRIVER_DEPTH 16
#define MAKISEGUI_BUFFER_DEPTH 4
#define MAKISEGUI_BUFFER_DEPTHMASK 0b1111


#define MAKISE_BUF_H 16
#define MAKISE_BUF_W 240

#define MAKISE_GUI_INPUT_BUFFER_LEN 10


#ifdef MAKISE_ILI9340_USE
#include "ili9340.h"
#endif

#endif
