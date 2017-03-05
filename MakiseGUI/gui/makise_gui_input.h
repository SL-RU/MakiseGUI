#ifndef _MAKISE_H_G_Input
#define _MAKISE_H_G_Input 1

#include <stdint.h>

typedef enum
{
    M_KEY_OK         = 1   , //main select button
    M_KEY_LEFT             , //arrow cursor move
    M_KEY_RIGHT            , //arrow cursor move
    M_KEY_UP               , //arrow cursor move
    M_KEY_DOWN             , //arrow cursor move
    M_KEY_ABOUT            , //view some info
    M_KEY_PROPERTY         , //change or view some properties
    M_KEY_TAB_NEXT         , //choose the next tab if possible and required
    M_KEY_TAB_BACK         , //choose the previous tab if possible and required
#if MAKISE_GUI_INPUT_USE_CURSOR == 1
    M_CURSOR_KEY           , //when cursor was pressed, or user tapped on the screen
#endif
    M_KEY_USER_0     = 100 , //this and all the next keys can be used and defined by user
} MInputKeyEnum;

typedef enum
{
    M_INPUT_CLICK              = 1, //when button was released
    M_INPUT_PRESSING              , //when button is currently pressing
    M_INP_CHARACTER               , //when character was inputed
#if MAKISE_GUI_INPUT_USE_CURSOR == 1
    M_INPUT_MOVE                  , //when cursor was moved
#endif
} MInputEventEnum;

#if MAKISE_GUI_INPUT_USE_CURSOR == 1
typedef struct
{
    uint32_t x;
    uint32_t y;
    int16_t dx;
    int16_t dy;
} MInputCursorEvent;    
#endif
    
typedef struct
{
    MInputKeyEnum key;        //what's inputing now
    MInputEventEnum event; //additional information
    uint32_t time;         //how long key was pressed or is pressing
    char c;                //character, if key == M_KEY_ifndef
#if MAKISE_GUI_INPUT_USE_CURSOR == 1
    MInputCursorEvent cursor; //cursor's position
#endif    
} MInputData;
    
typedef enum
{
    M_INPUT_HANDLED,
    M_INPUT_NOT_HANDLED
} MInputResultEnum;
    
#include "makise_gui.h"
    
/**
 * Add event to the event buffer
 *
 * @param h GUI host structure
 * @param d Structure to send
 * @return 
 */
void makise_gui_input_send(MHost *h, MInputData d);
/**
 * Add button event to event's buffer 
 *
 * @param h GUI host structure
 * @param key Key
 * @param event M_INPUT_CLICK or M_INPUT_PRESSING
 * @param time time since pressing was started
 * @return 
 */
void makise_gui_input_send_button(MHost *h, MInputKeyEnum key, MInputEventEnum event, uint32_t time);

/**
 * 
 *
 * @param h 
 * @return 
 */
void makise_gui_input_perform(MHost *h);

/* TODO: custom event's result handling user-definable method */

#endif
