#ifndef MAKISE_PRIMITIVES_H
#define MAKISE_PRIMITIVES_H 1

#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
    MP_Point,
    MP_Clear,
    MP_Rectangle,
    MP_Circle,
    MP_Line,
    MP_Triangle
} MPrimitiveType;

typedef struct _MDPrimitive MDPrimitive;    
    
#include "makise.h"
#include <string.h>
#include <math.h>

#define MAX_POLY_CORNERS           200
#define MDPRIMITIVE_POINTS_LEN     3    

typedef struct 
{
    int16_t x;
    int16_t y;
} MPoint;

typedef struct _MDPrimitive {
    MPrimitiveType type;
    MPoint points[3];
    int32_t points_len;
    int32_t w;
    int32_t h;
    int32_t r;
    int32_t thickness;
    MColor color;
    MColor color_fill;
} MDPrimitive;


//*********************************************************************

/**
 * Fill whole buffer with color
 * 
 * @param[in] b buffer
 * @param[in] c color
 * @return 
 */
void makise_d_clear           ( const MakiseBuffer* b, MColor c);
/**
 * Draw point
 * 
 * @param[in] b buffer
 * @param[in] pos position
 * @param[in] c color
 * @return 
 */    
void makise_d_point           ( const MakiseBuffer* b,
                                MPoint pos,
                                MColor c );
/**
 * Draw line
 * 
 * @param[in] b buffer
 * @param[in] start start position
 * @param[in] end end position
 * @param[in] c color
 * @return 
 */
void makise_d_line            ( const MakiseBuffer* b,
                                MPoint start,
                                MPoint end,
                                uint16_t thickness, MColor c );
/**
 * Draw rectangle without filling
 * 
 * @param[in] b buffer
 * @param[in] pos position
 * @param[in] w width
 * @param[in] h height
 * @param[in] c color
 * @return 
 */
void makise_d_rect            ( const MakiseBuffer* b,
                                MPoint pos,
                                uint16_t w, uint16_t h,
                                uint16_t thickness, MColor c );
/**
 * Draw rectangle with filling
 * 
 * @param[in] b buffer
 * @param[in] pos position
 * @param[in] w width
 * @param[in] h height
 * @param[in] c color
 * @param[in] c_fill filling color
 * @return 
 */
void makise_d_rect_filled     ( const MakiseBuffer* b,
                                MPoint pos,
                                uint16_t w, uint16_t h,
                                uint16_t thickness,
                                MColor c, MColor c_fill );
/**
 * Draw rectangle with rounded corners with filling
 * 
 * @param[in] b buffer
 * @param[in] pos position
 * @param[in] w width
 * @param[in] h height
 * @param[in] r radius of corners
 * @param[in] c color
 * @param[in] c_fill filling color
 * @return 
 */
void makise_d_rect_rounded    ( const MakiseBuffer* b,        
                                MPoint pos,  
                                uint16_t w, uint16_t h,
                                uint16_t r,
                                uint16_t thickness, 
                                MColor c, MColor c_fill );
/**
 * Draw circle
 * 
 * @param[in] b buffer
 * @param[in] pos position
 * @param[in] r radius
 * @param[in] c color
 * @return 
 */
void makise_d_circle          ( const MakiseBuffer* b,
                                MPoint pos,
                                uint16_t r,
                                uint16_t thickness, 
                                MColor c );
/**
 * Draw disk with circle
 * 
 * @param[in] b buffer
 * @param[in] pos position
 * @param[in] r radius
 * @param[in] c circle color
 * @param[in] c_fill filling color
 * @return 
 */
void makise_d_circle_filled   ( const MakiseBuffer* b,
                                MPoint pos,
                                uint16_t r,
                                uint16_t thickness,
                                MColor c, MColor c_fill );
/**
 * Draw zigzag line
 * 
 * @param[in] b buffer
 * @param[in] points array of points
 * @param[in] count  count of items in array
 * @param[in] c color
 * @return 
 */
void makise_d_polyline        ( const MakiseBuffer* b,
                                const MPoint* points, uint32_t count,
                                uint16_t thickness,
                                MColor c );
/**
 * Draw zigzag line with translation and rotation
 * 
 * @param[in] b buffer
 * @param[in] pos translation position
 * @param[in] rot rotation angle in radians
 * @param[in] points array of points
 * @param[in] count  count of items in array
 * @param[in] c color
 * @return 
 */    
void makise_dex_polyline      ( const MakiseBuffer* b,
                                MPoint pos,
                                double rot,
                                const MPoint* points, uint32_t count,
                                uint16_t thickness,
                                MColor c );
/**
 * Draw triangle
 * 
 * @param[in] b buffer
 * @param[in] p0 first point
 * @param[in] p1 second point
 * @param[in] p2 third point
 * @param[in] c color
 * @param[in] c_fill filling color
 * @return 
 */
void makise_d_triangle_filled ( const MakiseBuffer* b,
                                MPoint p0,
                                MPoint p1,
                                MPoint p2,
                                uint16_t thickness,
                                MColor c, MColor c_fill );

//**************************************************************************

/**
 * Internal function. Calls buffer's drawer to draw the primitive from it's data 
 * 
 * @param[in] b buffer
 * @param[in] p primitive data
 * @return 
 */
void makise_d_primitive(const MakiseBuffer *b, const MDPrimitive *p);

#ifdef MAKISE_PRIMITIVES_DRAWER_DEFAULT    
/**
 * Internal function. Default drawer. Simply draws primitives into buffer pixel by pixel without anti-aliasing or any advanced features and without any accelerators. To enable it define MAKISE_PRIMITIVES_DRAWER_DEFAULT in makise_config.h and set drawer to 0 in makise_autoinit(...);
 * 
 * @param[in] b buffer
 * @param[in] p primitive data
 * @return 
 */    
MResult makise_primitives_default_drawer ( const MakiseBuffer* b,
                                           const MDPrimitive *primitive );
MColor makise_pget(const MakiseBuffer *b, uint32_t x, uint32_t y);
void   makise_pset(const MakiseBuffer *b, uint32_t x, uint32_t y, MColor c);

/**
 * get point without checking borders
 */
MColor makise_pget_fast(const MakiseBuffer *b, uint32_t x, uint32_t y);
/**
 * set point without checking borders
 */
void   makise_pset_fast(const MakiseBuffer *b, uint32_t x, uint32_t y, MColor c);    
#endif
    
#ifdef __cplusplus
}
#endif

#endif /* LCDHAL_H_ */
