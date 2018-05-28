#include "makise_primitives.h"

#ifdef MAKISE_PRIMITIVES_DRAWER_DEFAULT

#define MAX_POLY_CORNERS   200


#define MSWAP(type, i, j) {type t = i; i = j; j = t;}
#define MMIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MMAX(X, Y) (((X) > (Y)) ? (X) : (Y))

void makise_pdd_line (const MakiseBuffer* b,
                      int16_t x0, int16_t y0,
                      int16_t x1, int16_t y1, MColor c );

uint32_t kpset, kpset32, kpsett;
inline uint32_t makise_pget(const MakiseBuffer *b, uint32_t x, uint32_t y)
{
    if((x) < (b)->width && (y) < (b)->height)
    {							
	kpset = ((y)*((b)->width) + (x)) * (b)->pixeldepth;
	kpset32 = kpset/32;
	return (b)->depthmask &
            (((uint32_t*)(b)->buffer)[kpset32] >> (kpset-(kpset32)*32));
    }						
    return 0;
}

inline void makise_pset(const MakiseBuffer *b, uint32_t x, uint32_t y, uint32_t c)
{
    if((x) < b->border.ex && (y) < b->border.ey &&
       (x) >= b->border.x && (y) >= b->border.y)		
    {									
	kpset = ((y)*((b)->width) + (x)) * (b)->pixeldepth;
	kpset32 = kpset/32;
	kpsett = kpset - kpset32*32;

	if(((b)->depthmask & (((uint32_t*)(b)->buffer)[kpset32] >> (kpsett))) == c)
	    return;
	
	((uint32_t*)(b)->buffer)[kpset32] =
            (((uint32_t*)(b)->buffer)[kpset32] &
             ~((b)->depthmask << kpsett)) | (c << kpsett); 
    }
}
inline uint32_t makise_pget_fast(const MakiseBuffer *b, uint32_t x, uint32_t y)
{
    kpset = ((y)*((b)->width) + (x)) * (b)->pixeldepth;
    kpset32 = kpset/32;
    return (b)->depthmask &
        (((uint32_t*)(b)->buffer)[kpset32] >> (kpset-(kpset32)*32));
}

inline void makise_pset_fast(const MakiseBuffer *b, uint32_t x, uint32_t y, uint32_t c)
{
    kpset = ((y)*((b)->width) + (x)) * (b)->pixeldepth;
    kpset32 = kpset/32;
    kpsett = kpset - kpset32*32;
    
    if ( ( ( b )->depthmask &
           (((uint32_t*)(b)->buffer)[ kpset32 ] >> ( kpsett ) ) ) == c )
        return;
    
    ((uint32_t*)(b)->buffer)[kpset32] =
        (((uint32_t*)(b)->buffer)[kpset32] &
         ~((b)->depthmask << kpsett)) | (c << kpsett); 
}


//**************************************************************************************
void makise_pdd_clear(const MakiseBuffer* b, MColor c)
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

void makise_pdd_point(const MakiseBuffer* b, uint16_t x, uint16_t y, MColor c)
{
    if(c != MC_Transparent)
        makise_pset(b, x, y, c);
}
void makise_pdd_rect( const MakiseBuffer* b,
                      int16_t x,  int16_t y,
                      uint16_t w, uint16_t h,
                      MColor c)
{
    if ( x >= b->width || y >= b->height || c == MC_Transparent )
        return;

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
        w = b->width - x;
    if(y + h > b->height)
        h = b->height - y;
    
    if(w == 0 || h == 0)
        return;

    
    makise_pdd_line(b, x, y, x + w, y, c);
    makise_pdd_line(b, x, y + h, x + w, y + h, c);
    makise_pdd_line(b, x, y, x, y + h, c);
    makise_pdd_line(b, x + w, y, x + w, y + h, c);
    /*
      if depth = 4bits
      byte 1       byte 2
      coordinates |1,0   0,0| |3,0   2,0|
      binaryvalue |0000 0000| |0100 0101|
    */
}

void makise_pdd_rect_filled ( const MakiseBuffer* b,
                              int16_t x,  int16_t y,
                              uint16_t w, uint16_t h,
                              MColor c,
                              MColor c_fill )
{
    if ( c_fill == MC_Transparent ) {
        makise_pdd_rect( b, x, y, w, h, c );
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
        w = b->width - x;

    if ( y + h > b->height )
        h = b->height - y;
    
    if ( w == 0 || h == 0 )
        return;



    if ( c != MC_Transparent ) {
        makise_pdd_line( b, x, y, x + w, y, c );
        makise_pdd_line( b, x, y + h, x + w, y + h, c );
        makise_pdd_line( b, x, y, x, y + h, c );
        makise_pdd_line( b, x + w, y, x + w, y + h, c );
    }

    /*
      if depth = 4bits
      byte 1       byte 2
      coordinates |1,0   0,0| |3,0   2,0|
      binaryvalue |0000 0000| |0100 0101|
    */
    uint32_t is_tr = ( c != MC_Transparent );

    for (int i = y+1; i < h + y; i++) {
        makise_pdd_line(b, x + is_tr, i, x+w - is_tr, i, c_fill);
    }    
}

void makise_pdd_circle (const MakiseBuffer* b,
                        uint16_t xc, uint16_t yc,
                        uint16_t r, MColor c )
{
    if ( c == MC_Transparent ) return;
    
    int16_t x = 0;
    int16_t y = r;
    int16_t d = 5 - (4 * r);
    int16_t dA = 12, dB = 20 - 8 * r;

    while(x < y) {
        makise_pset(b, xc + x, yc + y, c);
        makise_pset(b, xc - x, yc + y, c);
        makise_pset(b, xc + x, yc - y, c);
        makise_pset(b, xc - x, yc - y, c);
        makise_pset(b, xc + y, yc + x, c);
        makise_pset(b, xc - y, yc + x, c);
        makise_pset(b, xc + y, yc - x, c);
        makise_pset(b, xc - y, yc - x, c);
        if (d < 0) { 
            d += dA;
            dB += 8;
        } else {
            y--;
            d += dB;
            dB += 16;
        }
        x++;
        dA += 8;
    }
    makise_pset(b, xc + x, yc + y, c);
    makise_pset(b, xc - x, yc + y, c);
    makise_pset(b, xc + x, yc - y, c);
    makise_pset(b, xc - x, yc - y, c);
}

/* One of Abrash's ellipse algorithms  */
void draw_ellipse(const MakiseBuffer* p,
                  int x, int y, int a, int b,
                  MColor color, MColor c_fill)
{
    int wx, wy;
    int thresh;
    int asq = a * a;
    int bsq = b * b;
    int xa, ya;

    makise_pset(p, x, y+b, color);
    makise_pset(p, x, y-b, color);

    wx = 0;
    wy = b;
    xa = 0;
    ya = asq * 2 * b;
    thresh = asq / 4 - asq * b;

    for (;;) {
        thresh += xa + bsq;

        if (thresh >= 0) {
            ya -= asq * 2;
            thresh -= ya;
            wy--;
        }

        xa += bsq * 2;
        wx++;

        if (xa >= ya)
          break;

         makise_pset(p, x+wx, y-wy, color);
         makise_pset(p, x-wx, y-wy, color);
         makise_pset(p, x+wx, y+wy, color);
         makise_pset(p, x-wx, y+wy, color);

    }

     makise_pset(p, x+a, y, color);
     makise_pset(p, x-a, y, color);

    wx = a;
    wy = 0;
    xa = bsq * 2 * a;

    ya = 0;
    thresh = bsq / 4 - bsq * a;

    for (;;) {
        thresh += ya + asq;

        if (thresh >= 0) {
            xa -= bsq * 2;
            thresh = thresh - xa;
            wx--;
        }

        ya += asq * 2;
        wy++;

        if (ya > xa)
          break;


         
         makise_pset(p, x+wx, y-wy, color);
         makise_pset(p, x-wx, y-wy, color);

         makise_pset(p, x+wx, y+wy, color);
         makise_pset(p, x-wx, y+wy, color);
    }
}

void makise_pdd_circle_filled (const MakiseBuffer* b,
                               uint16_t xc, uint16_t yc,
                               uint16_t r, MColor c, MColor c_fill )
{
    
    if ( c_fill == MC_Transparent ) {
        makise_pdd_circle( b, xc, yc, r, c );
        return;
    }


    int16_t x = 0;
    int16_t y = r;
    int16_t d = 5 - (4 * r);
    int16_t dA = 12, dB = 20 - 8 * r;
    //int16_t is_tr = (c != MC_Transparent);

    while ( x < y ) {
        if ( y != r ) {
            makise_pdd_line(b,
                            xc - x, yc + y,
                            xc + x, yc + y, c_fill);
            makise_pdd_line(b,
                            xc - x, yc - y,
                            xc + x, yc - y, c_fill);
        }
        if ( x != r ) {
            makise_pdd_line(b,
                            xc - y, yc + x,
                            xc + y, yc + x, c_fill);
            makise_pdd_line(b,
                            xc - y, yc - x,
                            xc + y, yc - x, c_fill);
        }

        if (d < 0) { 
            d += dA;
            dB += 8;
        } else {
            y--;
            d += dB;
            dB += 16;
        }
        x++;
        dA += 8;
    }

    makise_pdd_line(b,
                    xc - x, yc + y,
                    xc + x, yc + y, c_fill);
    makise_pdd_line(b,
                    xc - x, yc - y,
                    xc + x, yc - y, c_fill);
    
    makise_pdd_circle( b, xc, yc, r, c );
}

void makise_pdd_line (const MakiseBuffer* b,
                      int16_t x0, int16_t y0,
                      int16_t x1, int16_t y1, MColor c )
{
    if ( c == MC_Transparent )                  return;
    if ( x1 < 0 && x0 < 0 )                     return;
    if ( x1 >= b->width && x0 >= b->width )     return;
    if ( y1 < 0 && y0 < 0 )                     return;
    if ( y1 >= b->height && y0 >= b->height )   return;
    
    int dx, dy;
    if(x0 > x1) { MSWAP(int, x0, x1); MSWAP(int, y0, y1);  }
    dy = y1 - y0; dx = x1 - x0;
    if(dx == 0) {
        //if horizontal
        if(MMIN(y0, y1) < b->border.y || MMAX(y0, y1) > b->border.ey)
            return;
    } else {
        // LEFT BORDER
        if(x0 < b->border.x) {
            y0 = y0 + ((b->border.x - x0) * dy / dx);
            x0 = b->border.x;
        }
        // RIGHT BORDER
        dy = y1 - y0; dx = x1 - x0;
        if(x1 > b->border.ex) {
            y1 = y1 + ((b->border.ex - x1) * dy / dx);
            x1 = b->border.ex;
        }
    }

    // UPPER BORDER
    if(y0 > y1) { MSWAP(int, x0, x1); MSWAP(int, y0, y1);  }
    dy = y1 - y0; dx = x1 - x0;
    if(dy == 0) {
        //if vertical
        if(MMIN(x0, x1) < b->border.x || MMAX(x0, x1) > b->border.ex)
            return;
    } else {
        if(y0 < b->border.y) {
            x0 = x0 + ((b->border.y - y0) * dx / dy);
            y0 = b->border.y;
        }
        // BOTTOM BORDER
        dy = y1 - y0; dx = x1 - x0;
        if(y1 > b->border.ey) {
            x1 = x1 + ((b->border.ey - y1) * dx / dy);
            y1 = b->border.ey;
        }
    }
    
    int dirx = x0<x1 ? 1 : -1,
        diry = y0<y1 ? 1 : -1; 
    dy = abs( y0 - y1 );
    dx = abs( x0 - x1 );
    
    if ( dy == 0 ) {                                        // If line is horizontal we can memset it
        MColor C = 0;                                     // byte filled with colors for memset
        uint32_t kb, j;

        for (int i = 0; i < (32/b->pixeldepth); i++) {
            C |= (c & b->depthmask) << i*b->pixeldepth;
        }

        j = x0;
        while ((j % (32 / b->pixeldepth)) && j <= x1) { //coordinate position in the array must in the beginning of the byte
            makise_pset_fast(b, j, y1, c);
            j++;
        }

        kb = (y0*(b->width) + j) * (b)->pixeldepth/32;
        while ( j <= x1 && x1 - j >= 32/b->pixeldepth ) {
            ((uint32_t*)b->buffer)[kb] = C;
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

    int err = (dx>dy ? dx : -dy)/2, e2;
    
    for(;;){
        makise_pset_fast( b, x0, y0, c );
        if (x0==x1 && y0==y1) break;
        e2 = err;
        if (e2 >-dx) { err -= dy; x0 += dirx; }
        if (e2 < dy) { err += dx; y0 += diry; }
    }
}
/*
  Draw down part of triangle
  uy lx *-------------* rx
  \           /
  \         /
  \       /
  \     /
  \   /
  \ /
  * mx, my
  */
static void draw_down_tri (const MakiseBuffer*b,
                           int32_t uy, int32_t lx, int32_t rx,
                           int32_t mx, int32_t my, 
                           MColor c)
{
    if(lx > rx) MSWAP(int32_t, lx, rx);
    int32_t ldx = abs(lx - mx), rdx = abs(rx - mx),
        dy = abs(my - uy),
        rerr = 0, lerr = 0, re2 = 0, le2 = 0,
        ldirx = mx - lx < 0 ? -1 : 1, rdirx = mx - rx < 0 ? -1 : 1,
        diry = my - uy < 0 ? -1 : 1,
        lxx = lx, rxx = rx;
    ldirx = mx - lx == 0 ? 0 : ldirx;
    lerr = (ldx > dy ? ldx : -dy) / 2;
    
    rdirx = mx - rx == 0 ? 0 : rdirx;
    rerr = (rdx > dy ? rdx : -dy) / 2;

    diry = my - uy == 0 ? 0 : diry;
    if(diry == 0)
        return;
    
    int32_t y = uy;
    uint8_t lc = 1;
    for(;;) {

        if(y == my) break;
        re2 = rerr; le2 = lerr;
        if(le2 > -ldx && lc) { lerr -= dy; lxx += ldirx; }
        if(re2 > -rdx && !lc) { rerr -= dy; rxx += rdirx; }
        if(le2 < dy)
            lc = 0;
        if(le2 < dy && re2 < dy) {
            if(diry == 1)
                makise_pdd_line(b, lxx, y, rxx - (rxx - lxx > 2 ? 1 : 0), y, c);
            lc = 1;
            y += diry;
            lerr += ldx; rerr += rdx;
            if(diry == -1 && y != my)
                makise_pdd_line(b, lxx + (rxx - lxx > 2 ? 1 : 0), y, rxx - (rxx - lxx > 2 ? 1 : 0), y, c);
            
        }
    }
}

void makise_pdd_triangle_filled ( const MakiseBuffer*b,
                                  int32_t x0, int32_t y0,
                                  int32_t x1, int32_t y1,
                                  int32_t x2, int32_t y2,
                                  MColor c, MColor c_fill)
{
    if(b == 0 || (c == MC_Transparent && c_fill == MC_Transparent) ) return;
    
    if(y0 > y1) { MSWAP(int, x0, x1); MSWAP(int, y0, y1); }
    if(y0 > y2) { MSWAP(int, x0, x2); MSWAP(int, y0, y2); }
    int32_t uy = y0, ux = x0, //upper point
        mx, my, // middle point
        bx, by; //bottom point
    if(y1 < y2) {
        mx = x1; my = y1;
        bx = x2; by = y2;
    } else {
        mx = x2; my = y2;
        bx = x1; by = y1;
    }

    int32_t mmx = bx - ux;
    mmx = ux + (mmx * my - mmx * uy) / (by - uy);

    draw_down_tri(b, my, mx, mmx, ux, uy, c_fill);
    draw_down_tri(b, my, mx, mmx, bx, by, c_fill);
    
    makise_pdd_line(b, x0, y0, x1, y1, c);
    makise_pdd_line(b, x0, y0, x2, y2, c);
    makise_pdd_line(b, x2, y2, x1, y1, c);
}

void _makise_pdd_rect_rounded(const MakiseBuffer* b,        
                              int16_t  xc, int16_t yc,  
                              uint16_t w, uint16_t h,
                              uint16_t r,
                              MColor c,             
                              MColor c_fill,
                              uint8_t it)
{
    int16_t x = 0;
    int16_t y = r;
    int16_t d = 5 - (4 * r);
    int16_t dA = 12, dB = 20 - 8 * r;
    
    while(x <= y)
    {
        if(it) {
            makise_pdd_line(b,
                            xc - x + r,     yc + y + h - r - 1,
                            xc + x + w - r - 1, yc + y + h - r - 1,
                            c_fill);
            makise_pdd_line(b,
                            xc - x + r,     yc - y + r,
                            xc + x + w - r - 1, yc - y + r,
                            c_fill);
            makise_pdd_line(b,
                            xc - y + r,     yc + x + h - r - 1,
                            xc + y + w - r - 1, yc + x + h - r - 1,
                            c_fill);
            makise_pdd_line(b,
                            xc - y + r,     yc - x + r,
                            xc + y + w - r - 1, yc - x + r,
                            c_fill);
        } else {
            makise_pset(b, xc + x + w - r - 1, yc + y + h - r - 1, c);
            makise_pset(b, xc + y + w - r - 1, yc + x + h - r - 1, c);
        
            makise_pset(b, xc - x + r, yc + y + h - r - 1, c);
            makise_pset(b, xc - y + r, yc + x + h - r - 1, c);
        
            makise_pset(b, xc + x + w - r - 1, yc - y + r, c);
            makise_pset(b, xc + y + w - r - 1, yc - x + r, c);
        
            makise_pset(b, xc - x + r, yc - y + r, c);
            makise_pset(b, xc - y + r, yc - x + r, c);
        }
        
        if (d < 0) { 
            d += dA;
            dB += 8;
        } else {
            y--;
            d += dB;
            dB += 16;
        }
        x++;
        dA += 8;
    }
}

void makise_pdd_rect_rounded( const MakiseBuffer* b,        
                              int16_t  xc, int16_t yc,  
                              uint16_t w, uint16_t h,
                              uint16_t r,
                              MColor c,             
                              MColor c_fill )
{
    if(b == 0 || (w == 0 && h == 0) ) return;
    
    if(r == 0) {
        makise_pdd_rect_filled(b, xc, yc, w, h, c, c_fill);
        return;
    }
    
    if(c_fill != MC_Transparent)
        _makise_pdd_rect_rounded(b, xc, yc, w, h, r, c, c_fill, 1);
    if(c != MC_Transparent && c != c_fill)
        _makise_pdd_rect_rounded(b, xc, yc, w, h, r, c, c_fill, 0);
    
    //center fill
    makise_pdd_rect_filled(b, xc, yc + r, w, h - 2 * r, c_fill, c_fill);
    
    //horizontal border
    makise_pdd_line(b,
                  xc + r - 2,     yc,
                  xc + w - r + 1, yc, c);
    makise_pdd_line(b,
                  xc + r - 2,     yc + h - 1,
                  xc + w - r + 1, yc + h - 1, c);
    //vertical border
    makise_pdd_line(b, xc, yc + r, xc, yc + h - r, c);
    makise_pdd_line(b, xc + w - 1, yc + r, xc + w - 1, yc + h - r, c);

}

MResult makise_primitives_default_drawer(const MakiseBuffer* b, const MDPrimitive *p)
{
    if(b == 0 || p == 0)
        return M_ZERO_POINTER;

    switch(p->type) {
    case MP_Clear: makise_pdd_clear(b, p->color); break;
    case MP_Point: makise_pdd_point(b,
                                    p->points[0].x, p->points[0].y,
                                    p->color);
        break;
    case MP_Rectangle:
        makise_pdd_rect_rounded(b,
                                p->points[0].x, p->points[0].y,
                                p->w, p->h, p->r, p->color, p->color_fill);
        break;
    case MP_Line: makise_pdd_line
            ( b,
              p->points[0].x, p->points[0].y,
              p->points[1].x, p->points[1].y,
              p->color);
        break;
    case MP_Circle: makise_pdd_circle_filled
            ( b,
              p->points[0].x, p->points[0].y,
              p->r,
              p->color, p->color_fill );
        break;
    case MP_Triangle: makise_pdd_triangle_filled
            ( b,
              p->points[0].x, p->points[0].y,
              p->points[1].x, p->points[1].y,
              p->points[2].x, p->points[2].y,
              p->color, p->color_fill );
        break;                        
    default: break;
    }
    
    return M_OK;
}
#endif
