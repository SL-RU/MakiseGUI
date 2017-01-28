#ifndef MAKISE_H
#define MAKISE_H 1

typedef struct _MakiseGUI MakiseGUI;
typedef struct _MakiseBuffer MakiseBuffer;
typedef struct _MakiseDriver MakiseDriver;

#define M_OK            1
#define M_ERROR         2
#define M_ZERO_POINTER  3


#include <stdlib.h>
#include "makise_config.h"
#include "makise_colors.h"
#include "makise_text.h"
#include "makise_primitives.h"
//#include "makise_gui.h"
#include "stm32f4xx_hal.h"

typedef struct _MakiseBuffer
{
    MakiseGUI *gui;
    uint16_t height;     //real height
    uint16_t width;      //real width 
    uint16_t pixeldepth; //smaller or equl to real depth
    uint8_t depthmask;  //mask like 0b111 for depth 3bit
    uint32_t * buffer;    //virtual buffer
    uint32_t size;       //size of the buffer
} MakiseBuffer;

typedef struct _MakiseDriver
{
    MakiseGUI *gui;
    uint16_t lcd_height; //real height
    uint16_t lcd_width;  //real width
    uint16_t buffer_height; //small buffer
    uint16_t buffer_width; //small buffer
    uint16_t pixeldepth; //real pixeldepth
    uint32_t * buffer;   //floating buffer
    uint32_t size;       //size of the buffer
    uint16_t posx;       //last pos of floating buffer
    uint16_t posy;       //last pos of floating buffer
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

//if partial_render = 0, then entire buffer will be rendered, if == 1, then will be rendered only first part, if == 2 then will be rendered second part
void makise_render(MakiseGUI *gui, uint8_t partial_render);

#endif
