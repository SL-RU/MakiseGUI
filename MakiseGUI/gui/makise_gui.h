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

#define MFocusEnum_NEXTPREV_MASK 0b00010000
#define MFocusEnum_NEXT_MASK     0b00100000
#define MFocusEnum_PREV_MASK     0b01000000
#define MFocusEnum_GET_MASK      0b00000100     
#define MFocusEnum_LEAVE_MASK    0b00001000         
typedef enum
{
    M_G_FOCUS_ERROR      = 0          ,
    M_G_FOCUS_OK         = 0b00000001 ,
    M_G_FOCUS_NOT_NEEDED = 0b00000010 ,
    M_G_FOCUS_NO         = 0b00000011 ,
    
    M_G_FOCUS_LEAVE      = MFocusEnum_LEAVE_MASK ,
    M_G_FOCUS_GET        = MFocusEnum_GET_MASK   ,
    
    M_G_FOCUS_NEXT       = MFocusEnum_NEXT_MASK | MFocusEnum_NEXTPREV_MASK,
    M_G_FOCUS_PREV       = MFocusEnum_PREV_MASK | MFocusEnum_NEXTPREV_MASK,
    
    M_G_FOCUS_GET_PREV   = MFocusEnum_PREV_MASK
                         | MFocusEnum_NEXTPREV_MASK 
                         | MFocusEnum_GET_MASK , // called after GET if focus was switched using focus_prev() function
    M_G_FOCUS_GET_NEXT   = MFocusEnum_NEXT_MASK
                         | MFocusEnum_NEXTPREV_MASK 
                         | MFocusEnum_GET_MASK , //called after GET if focus was switched using focus_next() function
} MFocusEnum;

typedef struct _MHost MHost;

#include "makise.h"
#include "makise_style.h"
#include "makise_gui_input.h"
#include "makise_gui_container.h"
#include "makise_gui_elements.h"

#include "makise_gui_interface.h"    
    
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
void makise_gui_autoinit ( MHost *host,
                           MakiseGUI *gui,
                           MakiseDriver *driver,
                           void* (*get_buffer)(uint32_t size),
                           MInputData (*inp_handler)(MInputData d,
                                                     MInputResultEnum res),
                           void (*draw)(MakiseGUI* gui),
                           void (*predraw)(MakiseGUI* gui),
                           void (*update)(MakiseGUI* gui),
                           MResult  ( *drawer )(
                               const MakiseBuffer*,
                               const MDPrimitive* ),
                           MakiseTextDrawer *text_drawer);

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
