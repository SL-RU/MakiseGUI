#ifndef MAKISE_H
#define MAKISE_H 1

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _MakiseGUI MakiseGUI;
typedef struct _MakiseBuffer MakiseBuffer;
typedef struct _MakiseDriver MakiseDriver;

#define M_OK            1
#define M_ERROR         2
#define M_ZERO_POINTER  3

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "makise_config.h"
#include "makise_colors.h"
#include "makise_text.h"
#include "makise_primitives.h"

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
    uint32_t *buffer;       //virtual buffer
    uint32_t size;          //size of the buffer

    MakiseBufferBorder border; //allowed region for new drawing
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
    
    uint8_t (*init) (MakiseGUI* gui);
    uint8_t (*start)(MakiseGUI* gui);
    uint8_t (*sleep)(MakiseGUI* gui);
    uint8_t (*awake)(MakiseGUI* gui);
    uint8_t (*set_backlight)(MakiseGUI* gui, uint8_t);
} MakiseDriver;

typedef struct _MakiseGUI
{
    MakiseBuffer* buffer;
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
uint8_t makise_start(MakiseGUI * gui);
uint32_t makise_pget(MakiseBuffer *b, uint16_t x, uint16_t y);
void makise_pset(MakiseBuffer *b, uint16_t x, uint16_t y, uint32_t c);

/**
 * get point without checking borders
 */
uint32_t makise_pget_fast(MakiseBuffer *b, uint16_t x, uint16_t y);
/**
 * set point without checking borders
 */
void makise_pset_fast(MakiseBuffer *b, uint16_t x, uint16_t y, uint32_t c);

//if partial_render = 0, then entire buffer will be rendered, if == 1, then will be rendered only first part, if == 2 then will be rendered second part
void makise_render(MakiseGUI *gui, uint8_t partial_render);

/**
 * set new border. This region must be smaller then previous. It will be cropped.
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
