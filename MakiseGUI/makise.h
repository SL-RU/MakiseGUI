#ifndef MAKISE_H
#define MAKISE_H 1

#ifdef __cplusplus
extern "C" {
#endif

    
typedef struct _MakiseGUI MakiseGUI;
typedef struct _MakiseBuffer MakiseBuffer;
typedef struct _MakiseDriver MakiseDriver;

typedef enum {
    M_OK          ,
    M_ERROR       ,
    M_ZERO_POINTER,
} MResult;

#include <stdlib.h>
#include <stdint.h>
#include "makise_config.h"
#include "makise_colors.h"
#include "makise_text.h"
#include "makise_primitives.h"
#include "makise_bitmap.h"


//if DEBUG OUTPUT disabled or MAKISE_DEBUG_OUTPUT isn't defined
//if you want debug output define that in makise_config.h:
//#define MAKISE_DEBUG_OUTPUT printf
#ifndef MAKISE_DEBUG_OUTPUT 
#define MAKISE_DEBUG_OUTPUT(...)
#endif
#ifndef MAKISE_ERROR_OUTPUT 
#define MAKISE_ERROR_OUTPUT MAKISE_DEBUG_OUTPUT
#endif    
#ifndef MAKISE_UNICODE
#define MAKISE_UNICODE 0
#endif
//if MAKISEGUI_DISPLAY_INVERTED == 1 then MC_BLACK = 1
#ifndef MAKISEGUI_DISPLAY_INVERTED
#define MAKISEGUI_DISPLAY_INVERTED 0
#endif

#if defined(MAKISE_MUTEX) && MAKISE_MUTEX
#define MAKISE_MUTEX_REQUEST(x) m_mutex_request_grant(x)
#define MAKISE_MUTEX_RELEASE(x) m_mutex_release_grant(x)    
#else
#define MAKISE_MUTEX_REQUEST(...)
#define MAKISE_MUTEX_RELEASE(...)    
#endif
    
typedef struct
{
    int32_t x;
    int32_t y;
    uint32_t w;
    uint32_t h;

    //will be counted by system
    int32_t ex; //border's end. doesn't included in the region
    int32_t ey; //border's end. doesn't included in the region
} MakiseBufferBorder;

typedef struct 
{
    MakiseBufferBorder border;
    MakiseBufferBorder last_border;
} MakiseBufferBorderData;

typedef struct _MakiseBuffer
{
    MakiseGUI *gui;
    uint16_t height;        //real height
    uint16_t width;         //real width
    uint32_t pixeldepth;    //smaller or equals to real depth
    uint32_t depthmask;     //example: mask like 0b111 is for depth 3bit
    void     *buffer;       //virtual buffer
    uint32_t size;          //size of the buffer

    MakiseBufferBorder border; //allowed region for new drawing

    MResult  ( *drawer )(
        const MakiseBuffer*,
        const MDPrimitive* ); // primitive drawer. See documentation. Or use &makise_primitives_default_drawer
    MakiseTextDrawer *text_drawer;
} MakiseBuffer;

typedef struct _MakiseDriver
{
    MakiseGUI *gui;
    uint16_t lcd_height;    //real height
    uint16_t lcd_width;     //real width
    uint16_t buffer_height; //small buffer
    uint16_t buffer_width;  //small buffer
    uint16_t pixeldepth;    //real pixeldepth
    uint32_t *buffer;       //floating buffer
    uint32_t size;          //size of the buffer
    uint16_t posx;          //last pos of floating buffer
    uint16_t posy;          //last pos of floating buffer
} MakiseDriver;

typedef struct _MakiseGUI
{
    MakiseBuffer *buffer; //to have backwards compatability
    MakiseBuffer _buffer;
    MakiseDriver* driver;
    void (*draw)(MakiseGUI* gui);
    void (*predraw)(MakiseGUI* gui);
    void (*update)(MakiseGUI* gui);
} MakiseGUI;

/**
 * Initialize gui and buffer structures.
 *
 * @param gui pointer to empty gui structure
 * @param driver poiter to initialized driver structure
 * @param buffer pointer to the empty buffer structure
 * @return size in bytes of required byte's array in buffer structure. And zero if was any error. You need to malloc buffer->buffer
 */
uint32_t makise_init(MakiseGUI * gui, MakiseDriver* driver, MakiseBuffer* buffer);
void makise_deinit(MakiseGUI* gui);


//if partial_render = 0, then entire buffer will be rendered, if == 1, then will be rendered only first part, if == 2 then will be rendered second part
void makise_render(MakiseGUI *gui, uint8_t partial_render);

/**
 * set new border. This region must be smaller then previous. Else it will be cropped.
 * Borders need for drawing GUI. For gui elements do not leave their & their parent's
 * borders.
 * After setting border & drawing it you need to call makise_rem_border.
 * 
 * @param buffer 
 * @param b new region
 * @return Border data needed to restore previous border. Needed for makise_rem_border
 */
MakiseBufferBorderData makise_add_border(MakiseBuffer *buffer, MakiseBufferBorder b);
void makise_rem_border(MakiseBuffer *buffer, MakiseBufferBorderData b);


#ifdef __cplusplus
}
#endif

#endif
