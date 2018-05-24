#ifndef MAKISE_PRIMITIVES_H
#define MAKISE_PRIMITIVES_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include "makise.h"
#include <string.h>
#include <math.h>

#define MAX_POLY_CORNERS   200

typedef struct 
{
    int16_t x;
    int16_t y;
} MakisePoint;

//void LCD_ASCII(V_FONT * vFont);

//*********************************************************************
void makise_d_clear(MakiseBuffer* b, uint32_t c);

void makise_d_point(MakiseBuffer* b, uint16_t x, uint16_t y, uint32_t c);

void makise_d_rect(MakiseBuffer* b, int16_t x, int16_t y, uint16_t w,
		   uint16_t h, uint32_t c);
void makise_d_rect_filled(MakiseBuffer* b, int16_t x, int16_t y, uint16_t w,
			  uint16_t h, uint32_t c, uint32_t fill_c);

void makise_d_circle(MakiseBuffer* b, uint16_t x, uint16_t y, uint16_t r, uint32_t c);
void makise_d_circle_filled(MakiseBuffer* b, uint16_t x, uint16_t y, uint16_t r, uint32_t c, uint32_t fill_c);

void makise_d_line(MakiseBuffer* b, int16_t x1, int16_t y1,
		   int16_t x2, int16_t y2, uint32_t c);
void makise_d_polyline(MakiseBuffer*b, MakisePoint* points, uint32_t count, uint32_t c);
void makise_dex_polyline(MakiseBuffer*b, int32_t x, int32_t y, double rot, MakisePoint* points, uint32_t count, uint32_t c);

void makise_d_triangle_filled(MakiseBuffer*b, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t c, uint32_t c_fill);

void makise_d_rect_rounded( MakiseBuffer* b,        
                            int16_t  x, int16_t y,  
                            uint16_t w, uint16_t h,
                            uint16_t r,
                            uint32_t c,             
                            uint32_t fill_c );    
//**************************************************************************

#ifdef __cplusplus
}
#endif

#endif /* LCDHAL_H_ */
