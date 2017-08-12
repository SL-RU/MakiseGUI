#include "makise_primitives.h"

#define MAX_POLY_CORNERS   200





//**************************************************************************************
void makise_d_clear(MakiseBuffer* b, uint32_t c)
{
    if(c == MC_Transparent)
	return;
    
    uint8_t k = 0;
    for (int i = 0; i < (8/b->pixeldepth); i++)
    {
	k |= (c & b->depthmask) << i*b->pixeldepth;
    }
    memset(b->buffer, k, b->size);
}

void makise_d_point(MakiseBuffer* b, uint16_t x, uint16_t y, uint32_t c)
{
    if(c != MC_Transparent)
	makise_pset(b, x, y, c);
}
void makise_d_rect(MakiseBuffer* b, int16_t x, int16_t y, uint16_t w,
		   uint16_t h, uint32_t c)
{
    if(x >= b->width || y >= b->height || c == MC_Transparent)
	return;
    if(x < 0)
    {
	if(-x > w)
	    return;
	w = w + x;
	x = 0;
    }
    if(y < 0)
    {
	if(-y > h)
	    return;
	h = h + y;
	y = 0;
    }
    if(x + w >= b->width)
	w = b->width - x - 1;
    if(y + h >= b->height)
	h = b->height - y - 1;
    
    if(w == 0 || h == 0)
	return;


    w-=1;
    h-=1;
    
    makise_d_line(b, x, y, x + w, y, c);
    makise_d_line(b, x, y + h, x + w, y + h, c);
    makise_d_line(b, x, y, x, y + h, c);
    makise_d_line(b, x + w, y, x + w, y + h, c);
    /*
      if depth = 4bits
      byte 1       byte 2
      coordinates |1,0   0,0| |3,0   2,0|
      binaryvalue |0000 0000| |0100 0101|
    */
}
void makise_d_rect_filled(MakiseBuffer* b, int16_t x, int16_t y, uint16_t w,
			  uint16_t h, uint32_t c, uint32_t fill_c)
{
    if ( fill_c == MC_Transparent ) {
        makise_d_rect(b, x, y, w, h, c);
        return;
    }

    if ( x >= b->width || y >= b->height ) return;

    if ( x < 0 ) {
        if ( -x > w ) return;
        w = w + x;
        x = 0;
    }

    if ( y < 0 ) {
        if ( -y > h ) return;
        h = h + y;
        y = 0;
    }

    if ( x + w > b->width )
        w = b->width - x - 1;

    if ( y + h > b->height )
        h = b->height - y - 1;
    
    if ( w == 0 || h == 0 )
        return;

    h -= 1;
    w -= 1;

    if ( c != MC_Transparent ) {
    makise_d_line( b, x, y, x + w, y, c );
    makise_d_line( b, x, y + h, x + w, y + h, c );
    makise_d_line( b, x, y, x, y + h, c );
    makise_d_line( b, x + w, y, x + w, y + h, c );
    }
    /*
      if depth = 4bits
                    byte 1       byte 2
      coordinates |1,0   0,0| |3,0   2,0|
      binaryvalue |0000 0000| |0100 0101|
    */
    uint32_t is_tr = (c != MC_Transparent);
    for (int i = y+1; i < h + y; i++) {
	makise_d_line(b, x + is_tr, i, x+w - is_tr, i, fill_c);
    }    
}
void makise_d_circle(MakiseBuffer* b, uint16_t xc, uint16_t yc, uint16_t r, uint32_t c)
{
    if(c == MC_Transparent)
	return;
    
    //middle point algorythm
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    makise_pset(b, xc, yc + r, c);
    makise_pset(b, xc, yc - r, c);
    makise_pset(b, xc + r, yc, c);
    makise_pset(b, xc - r, yc, c);

    while(x < y)
    {
	// ddF_x == 2 * x + 1;
	// ddF_y == -2 * y;
	// f == x*x + y*y - r*r + 2*x - y + 1;
	if(f >= 0)
	{
	    y--;
	    ddF_y += 2;
	    f += ddF_y;
	}
	x++;
	ddF_x += 2;
	f += ddF_x;
	makise_pset(b, xc + x, yc + y, c);
	makise_pset(b, xc - x, yc + y, c);
	makise_pset(b, xc + x, yc - y, c);
	makise_pset(b, xc - x, yc - y, c);
	makise_pset(b, xc + y, yc + x, c);
	makise_pset(b, xc - y, yc + x, c);
	makise_pset(b, xc + y, yc - x, c);
	makise_pset(b, xc - y, yc - x, c);
    }
    
}
void makise_d_circle_filled(MakiseBuffer* b, uint16_t xc, uint16_t yc, uint16_t r, uint32_t c, uint32_t fill_c)
{
    if(fill_c == MC_Transparent)
    {
	makise_d_circle(b, xc, yc, r, c);
	return;
    }
    //middle point algorythm
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;
    int16_t is_tr = (c != MC_Transparent);
    makise_d_line(b, xc - r, yc, xc + r, yc, fill_c);
    makise_pset(b, xc, yc + r, c);
    makise_pset(b, xc, yc - r, c);
    makise_pset(b, xc + r, yc, c);
    makise_pset(b, xc - r, yc, c);

    while(x < y)
    {
	// ddF_x == 2 * x + 1;
	// ddF_y == -2 * y;
	// f == x*x + y*y - r*r + 2*x - y + 1;
	if(f >= 0)
	{
	    y--;
	    ddF_y += 2;
	    f += ddF_y;
	}
	x++;
	ddF_x += 2;
	f += ddF_x;

	if(y != r)
	{
	    makise_d_line(b, xc - x + is_tr, yc + y, xc + x - is_tr, yc + y, fill_c);
	    makise_d_line(b, xc - x + is_tr, yc - y, xc + x - is_tr, yc - y, fill_c);
	}
	if(x != r)
	{
	    makise_d_line(b, xc - y + is_tr, yc + x, xc + y - is_tr, yc + x, fill_c);
	    makise_d_line(b, xc - y + is_tr, yc - x, xc + y - is_tr, yc - x, fill_c);
	}
	if(is_tr)
	{
	    makise_pset(b, xc + x, yc + y, c);
	    makise_pset(b, xc - x, yc + y, c);
	    makise_pset(b, xc + x, yc - y, c);
	    makise_pset(b, xc - x, yc - y, c);
	    makise_pset(b, xc + y, yc + x, c);
	    makise_pset(b, xc - y, yc + x, c);
	    makise_pset(b, xc + y, yc - x, c);
	    makise_pset(b, xc - y, yc - x, c);
	}
    }
    
}
void makise_d_line(MakiseBuffer* b, int16_t x0, int16_t y0,
		   int16_t x1, int16_t y1, uint32_t c)
{
    if(c == MC_Transparent)
	return;
    if(x1 < 0 && x0 < 0)
	return;
    if(x1 >= b->width && x0 >= b->width)
	return;
    if(y1 < 0 && y0 < 0)
	return;
    if(y1 >= b->height && y0 >= b->height)
	return;
    x1 = x1 < b->border.x ? b->border.x : x1;
    y1 = y1 < b->border.y ? b->border.y : y1;
    x0 = x0 < b->border.x ? b->border.x : x0;
    y0 = y0 < b->border.y ? b->border.y : y0;
    
    x0 =  x0 < b->border.ex-1 ? x0 : b->border.ex - 1;
    x1 =  x1 < b->border.ex-1 ? x1 : b->border.ex - 1;
    y0 =  y0 < b->border.ey-1 ? y0 : b->border.ey - 1;
    y1 =  y1 < b->border.ey-1 ? y1 : b->border.ey - 1;
    
    int16_t dy = 0;
    
    dy = abs(y0 - y1);				/* The difference between the y's */
    
    if(dy == 0) //if line is horizontal we can memset it
    {
	uint32_t C = 0; //byte filled with colors for memset
	uint32_t kb, j;
	    
	for (int i = 0; i < (32/b->pixeldepth); i++)
	{
	    C |= (c & b->depthmask) << i*b->pixeldepth; 
	}

	j = x0;
	while ((j % (32 / b->pixeldepth)) && j <= x1) { //coordinate position in the array must in the beginning of the byte
	    makise_pset_fast(b, j, y1, c);
	    j++;
	}

	
	kb = (y0*(b->width) + j) * (b)->pixeldepth/32;
	while(j <= x1 && x1 - j >= 32/b->pixeldepth)
	{
	    b->buffer[kb] = C;
	    kb += 1;
	    j += 32/b->pixeldepth;
	}
	j-=1;
	while (j <= x1) {
	    makise_pset_fast(b, j, y0, c);
	    j++;
	}
	return;
    }

    int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
    int sy = y0<y1 ? 1 : -1; 
    int err = (dx>dy ? dx : -dy)/2, e2;
 
    for(;;){
	makise_pset_fast(b, x0, y0, c);
	if (x0==x1 && y0==y1) break;
	e2 = err;
	if (e2 >-dx) { err -= dy; x0 += sx; }
	if (e2 < dy) { err += dx; y0 += sy; }
    }
}
void makise_d_polyline(MakiseBuffer*b, MakisePoint* points, uint32_t count, uint32_t c)
{
    if(count == 0 || points == 0 || c == MC_Transparent)
	return;

    uint32_t i = 0;

    for(i = 1; i < count; i++)
    {
	makise_d_line(b, points[i - 1].x, points[i - 1].y, points[i].x, points[i].y, c);
    }
}

void makise_dex_polyline(MakiseBuffer*b, int32_t x, int32_t y, double rot, MakisePoint* points, uint32_t count, uint32_t c)
{
    if(count == 0 || points == 0 || c == MC_Transparent)
	return;

    if(x == 0 && y == 0 && rot == 0)
	makise_d_polyline(b, points, count, c);
    
    uint32_t i = 0;
    float co = cos(rot), si = sin(rot);
    float lx, ly, cx, cy;

    lx = (points[0].x) * co - (points[0].y) * si + x;
    ly = (points[0].x) * si + (points[0].y) * co + y;
    for(i = 1; i < count; i++)
    {
	cx = (points[i].x) * co - (points[i].y) * si + x;
	cy = (points[i].x) * si + (points[i].y) * co + y;

	makise_d_line(b, lx , ly, cx, cy, c);
	lx = cx;
	ly = cy;
    }
    
}

void makise_d_triangle_filled(MakiseBuffer*b, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t c, uint32_t c_fill)
{
        
}
