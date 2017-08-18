#include <makise_bitmap.h>

void        makise_d_bitmap        ( MakiseBuffer *b,
                                     int16_t x, int16_t y,
                                     const MakiseBitmap *i,
                                     uint32_t c )
{
    uint32_t bitCounter, rawIndex, colIndex;
    const uint8_t * ptrByte;

    if(b->border.ex + b->border.w < x ||
       b->border.ey + b->border.h < y ||
       x <= b->border.x - i->width ||
       y <= b->border.y - i->height)
	return;
    
    ptrByte = i->data;
    bitCounter = 0;
    for(rawIndex = 0; rawIndex < i->height; rawIndex++)
    {
	for(colIndex = 0; colIndex < i->width; colIndex++)
	{
	    if (bitCounter > 7)
	    {
		bitCounter = 0;
		ptrByte++;
	    }
	    if(*ptrByte & (1<<bitCounter))
	    {
		makise_pset(b, x+colIndex, y+rawIndex, c);
	    }
	    bitCounter++;
	}
    }
}
