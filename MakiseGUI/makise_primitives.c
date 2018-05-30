#include "makise_primitives.h"

void makise_d_primitive       ( const MakiseBuffer *b, const MDPrimitive *p )
{
    if(b == 0 || p == 0) return;

    if(b->drawer == 0) return;

    b->drawer(b, p);
}

void makise_d_clear           ( const MakiseBuffer* b, MColor c)
{
    MDPrimitive p = {
        .type = MP_Clear,
        .points = {(MPoint){0}},
        .points_len = 0,
        .w = 0,
        .h = 0,
        .r = 0,
        .thickness = 0,
        .color = c,
        .color_fill = c
    };
    makise_d_primitive(b, &p);
}
void makise_d_point           ( const MakiseBuffer* b,
                                MPoint pos,
                                MColor c )
{
    MDPrimitive p = {
        .type = MP_Point,
        .points = {pos},
        .points_len = 1,
        .w = 0,
        .h = 0,
        .r = 0,
        .thickness = 0,
        .color = c,
        .color_fill = c
    };
    makise_d_primitive(b, &p);
}
void makise_d_line            ( const MakiseBuffer* b,
                                MPoint start,
                                MPoint end,
                                uint16_t thickness, MColor c )
{
    MDPrimitive p = {
        .type = MP_Line,
        .points = {start, end},
        .points_len = 2,
        .w = 0,
        .h = 0,
        .r = 0,
        .thickness = thickness,
        .color = c,
        .color_fill = c
    };
    makise_d_primitive(b, &p);
}
void makise_d_rect            ( const MakiseBuffer* b,
                                MPoint pos,
                                uint16_t w, uint16_t h,
                                uint16_t thickness, MColor c )
{
    MDPrimitive p = {
        .type = MP_Rectangle,
        .points = {pos},
        .points_len = 1,
        .w = w,
        .h = h,
        .r = 0,
        .thickness = thickness,
        .color = c,
        .color_fill = MC_Transparent
    };
    makise_d_primitive(b, &p);
}
void makise_d_rect_filled     ( const MakiseBuffer* b,
                                MPoint pos,
                                uint16_t w, uint16_t h,
                                uint16_t thickness,
                                MColor c, MColor c_fill )
{
    MDPrimitive p = {
        .type = MP_Rectangle,
        .points = {pos},
        .points_len = 1,
        .w = w,
        .h = h,
        .r = 0,
        .thickness = thickness,
        .color = c,
        .color_fill = c_fill
    };
    makise_d_primitive(b, &p);
}
void makise_d_rect_rounded    ( const MakiseBuffer* b,        
                                MPoint pos,  
                                uint16_t w, uint16_t h,
                                uint16_t r,
                                uint16_t thickness, 
                                MColor c, MColor c_fill )
{
    MDPrimitive p = {
        .type = MP_Rectangle,
        .points = {pos},
        .points_len = 1,
        .w = w,
        .h = h,
        .r = r,
        .thickness = thickness,
        .color = c,
        .color_fill = c_fill
    };
    makise_d_primitive(b, &p);
}
void makise_d_circle          ( const MakiseBuffer* b,
                                MPoint pos,
                                uint16_t r,
                                uint16_t thickness, 
                                MColor c )
{
    MDPrimitive p = {
        .type = MP_Circle,
        .points = {pos},
        .points_len = 1,
        .w = 0,
        .h = 0,
        .r = r,
        .thickness = thickness,
        .color = c,
        .color_fill = MC_Transparent
    };
    makise_d_primitive(b, &p);
}
void makise_d_circle_filled   ( const MakiseBuffer* b,
                                MPoint pos,
                                uint16_t r,
                                uint16_t thickness,
                                MColor c, MColor c_fill )
{
    MDPrimitive p = {
        .type = MP_Circle,
        .points = {pos},
        .points_len = 1,
        .w = 0,
        .h = 0,
        .r = r,
        .thickness = thickness,
        .color = c,
        .color_fill = c_fill
    };
    makise_d_primitive(b, &p);
}
void makise_d_triangle_filled ( const MakiseBuffer* b,
                                MPoint p0,
                                MPoint p1,
                                MPoint p2,
                                uint16_t thickness,
                                MColor c, MColor c_fill )
{
    MDPrimitive p = {
        .type = MP_Triangle,
        .points = {p0, p1, p2},
        .points_len = 3,
        .w = 0,
        .h = 0,
        .r = 0,
        .thickness = thickness,
        .color = c,
        .color_fill = c_fill
    };
    makise_d_primitive(b, &p);
}

/*---------------------------------------------------------------*/
void makise_d_polyline        ( const MakiseBuffer* b,
                                const MPoint* points, uint32_t count,
                                uint16_t thickness,
                                MColor c )
{
    if ( b == 0 || count == 0 || points == 0 || MC_IS_Transparent(c) ) return;
    
    uint32_t i = 0;
    
    for ( i = 1; i < count; i++ ) {
        makise_d_line( b,
                       (MPoint){points[i - 1].x, points[i - 1].y},
                       (MPoint){points[i].x    , points[i].y    }, thickness, c );
    }

}
void makise_dex_polyline      ( const MakiseBuffer* b,
                                MPoint pos,
                                double rot,
                                const MPoint* points, uint32_t count,
                                uint16_t thickness,
                                MColor c )
{
    if ( count == 0 || points == 0 || MC_IS_Transparent(c) ) return;

    if ( pos.x == 0 && pos.y == 0 && rot == 0)
        makise_d_polyline(b, points, count, thickness, c);
    
    uint32_t i = 0;
    float co = 1, si = 0;
    float lx, ly, cx, cy;
    
    if(rot != 0) {
        co = cos(rot);
        si = sin(rot);
        lx = (points[0].x) * co - (points[0].y) * si + pos.x;
        ly = (points[0].x) * si + (points[0].y) * co + pos.y;
    } else {
        lx = points[0].x + pos.x;
        ly = points[0].y + pos.y;
    }

    for ( i = 1; i < count; i++ ) {
        if(rot != 0) {
            cx = (points[i].x) * co - (points[i].y) * si + pos.x;
            cy = (points[i].x) * si + (points[i].y) * co + pos.y;
        } else {
            cx = points[i].x * co + pos.x;
            cy = points[i].y + pos.y;
        }

        makise_d_line(b,
                      (MPoint){lx ,ly},
                      (MPoint){cx, cy}, thickness, c);
        lx = cx;
        ly = cy;
    }
}
