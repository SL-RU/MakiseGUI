#ifndef _MAKISE_H_G
#define _MAKISE_H_G 1

#define M_G_CALL_DRAW         1
#define M_G_CALL_PREDRAW      2
#define M_G_CALL_UPDATE       3


typedef enum
{
    M_G_FOCUS_NO         = 0        ,
    M_G_FOCUS_OK         = 0b000001 ,
    M_G_FOCUS_NOT_NEEDED = 0b000010 ,
    M_G_FOCUS_ERROR      = 0b1000000,
    
    M_G_FOCUS_LEAVE      = 0b000100 ,
    M_G_FOCUS_GET        = 0b001000 ,
    
    M_G_FOCUS_NEXT       = 0b010000 ,
    M_G_FOCUS_PREV       = 0b100000 ,
    
    M_G_FOCUS_GET_PREV   = 0b101000 , //called after GET if focus was switched using focus_prev() function
    M_G_FOCUS_GET_NEXT   = 0b011000 , //called after GET if focus was switched using focus_next() function   
} MFocusEnum;

typedef struct _MHost MHost;

#include "makise.h"
#include "makise_gui_input.h"
#include "makise_gui_container.h"
#include "makise_gui_elements.h"

//Root container for all elements
typedef struct _MHost
{
    MContainer *host;    
    MInputData input_buffer[MAKISE_GUI_INPUT_BUFFER_LEN];
} MHost;


uint32_t makise_g_newid();
MFocusEnum makise_g_focus(MElement *el, MFocusEnum event);


MFocusEnum makise_g_host_focus_next(MHost *host);
MFocusEnum makise_g_host_focus_prev(MHost *host);
uint8_t makise_g_host_call   (MHost *host, uint8_t type);
MInputResultEnum makise_g_host_input  (MHost *host, MInputData data);
#endif
