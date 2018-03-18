#ifndef _MAKISE_H_G
#define _MAKISE_H_G 1

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    M_G_CALL_DRAW    = 1  ,
    M_G_CALL_PREDRAW      ,
    M_G_CALL_UPDATE       ,
} MElementCall;

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
#include "makise_style.h"
#include "makise_gui_input.h"
#include "makise_gui_container.h"
#include "makise_gui_elements.h"

//Root container for all elements
typedef struct _MHost
{
    MakiseGUI *gui;
    MContainer host;
    MPosition host_size;
    MInputHostData input;
#if MAKISE_MUTEX
    MAKISE_MUTEX_t mutex;
#endif    
} MHost;

/**
 * Only inits MHost structure. To init gui use makise_gui_autoinit()
 *
 * @param MHost MHost
 * @return 
 */
void makise_gui_init(MHost *host);
/**
 * 
 *
 * @param 
 * @param 
 * @return 
 */    
void makise_gui_autoinit(MHost *host,
		     MakiseGUI *gui,
		     MakiseDriver *driver,
		     uint32_t* (*get_buffer)(uint32_t size),
		     MInputData (*inp_handler)(MInputData d,
					       MInputResultEnum res),
		     void (*draw)(MakiseGUI* gui),
		     void (*predraw)(MakiseGUI* gui),
		     void (*update)(MakiseGUI* gui)		     
    );

uint32_t makise_g_newid();
/**
 * This method will focus required element and all it's parents. Other already focused elements will be leaved by focus
 *
 * @param el 
 * @param event 
 * @return 
 */
MFocusEnum makise_g_focus(MElement *el, MFocusEnum event);


/**
 * Change focus to next element in the host's focus queue
 *
 * @param host host
 * @return MFocusEnum
 */
MFocusEnum makise_g_host_focus_next(MHost *host);
/**
 * Change focus to previous element in the host's focus queue
 *
 * @param host host
 * @return MFocusEnum
 */
MFocusEnum makise_g_host_focus_prev(MHost *host);
    
/**
 * Call draw, predraw or update of all enabled elements in the host
 *
 * @param host host
 * @param gui MakiseGUI with target buffer
 * @param type MElementCall: M_G_CALL_PREDRAW, M_G_CALL_DRAW or M_G_CALL_UPDATE
 * @return M_OK, M_ERROR
 */
uint8_t makise_g_host_call(MHost *host, MakiseGUI *gui, MElementCall type);
MInputResultEnum makise_g_host_input(MHost *host, MInputData data);

void makise_g_print_tree(MHost *host);

#ifdef __cplusplus
}
#endif

#endif
