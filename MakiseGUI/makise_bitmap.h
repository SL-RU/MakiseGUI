#ifndef MAKISE_BITMAP_H
#define MAKISE_BITMAP_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include "makise.h"
#include "makise_config.h"    

typedef struct _MakiseBitmap {
    uint16_t            width;      // Common width.
    uint16_t            height;     // Common height.
    const uint8_t*      data;             // Bitmap data.
} MakiseBitmap;


// Draw single bitmap.
void makise_d_bitmap ( MakiseBuffer *b,
                       int32_t x, int32_t y,
                       const MakiseBitmap *i,
                       MColor c );

#ifdef __cplusplus
}
#endif //cpp

#endif //MAKISE_BITMAP_H
