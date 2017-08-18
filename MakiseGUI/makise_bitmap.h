#ifndef MAKISE_BITMAP_H
#define MAKISE_BITMAP_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include "makise.h"
#include "makise_config.h"    



typedef struct _MakiseBitmap {
    uint16_t            width;   //common width
    uint16_t            height;  //common height
    uint8_t             data[];    //bitmap data
} MakiseBitmap;


// Draw single bitmap.
void        makise_d_bitmap        ( MakiseBuffer *b,
                                     int16_t x, int16_t y,
                                     const MakiseBitmap *i,
                                     uint32_t c );

#ifdef __cplusplus
}
#endif //cpp

#endif //MAKISE_BITMAP_H
