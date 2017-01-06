//comment unused displays
#define MAKISE_ILI9340_USE 1

#define MAKISE_malloc(x) malloc(x)
#define MAKISEGUI_BUFFER_DEPTH 2
#define MAKISEGUI_BUFFER_DEPTHMASK 0b11


#define MAKISE_BUF_H 16
#define MAKISE_BUF_W 240

#ifdef MAKISE_ILI9340_USE
#include "ili9340.h"
#endif
