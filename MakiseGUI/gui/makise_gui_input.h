#ifndef _MAKISE_H_G_Input
#define _MAKISE_H_G_Input

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "makise_config.h"

typedef enum
{
    M_KEY_NONE       = 0   , //NO KEY
    M_KEY_OK         = 1   , //main select button
    M_KEY_LEFT             , //arrow cursor move
    M_KEY_RIGHT            , //arrow cursor move
    M_KEY_UP               , //arrow cursor move
    M_KEY_DOWN             , //arrow cursor move
    M_KEY_ABOUT            , //view some info
    M_KEY_PROPERTY         , //change or view some properties
    M_KEY_TAB_NEXT         , //choose the next tab if possible and required
    M_KEY_TAB_BACK         , //choose the previous tab if possible and required
#if MAKISE_GUI_INPUT_POINTER_ENABLE == 1
    M_KEY_CURSOR           , //when cursor was pressed, or user tapped on the screen
#endif
    M_KEY_USER_0     = 100 , //this and all the next keys can be used and defined by user
} MInputKeyEnum;

typedef enum
{
    M_INPUT_NONE               = 0,
    M_INPUT_CLICK              = 1, //when button was released
    M_INPUT_PRESSING              , //when button is currently pressing after first press
    M_INPUT_CHARACTER               , //when character was inputed
#if MAKISE_GUI_INPUT_POINTER_ENABLE == 1
    M_INPUT_MOVE                  , //when cursor was moved
    M_INPUT_FIRST_PRESS           , //ONLY FOR CURSOR input!!! when cursor touched element in the first time.
#endif
} MInputEventEnum;

#if MAKISE_GUI_INPUT_POINTER_ENABLE == 1
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
#if MAKISE_GUI_INPUT_POINTER_ENABLE == 1
    MInputCursorEvent cursor; //cursor's position
#endif    
} MInputData;
    
typedef enum
{
    M_INPUT_HANDLED,
    M_INPUT_NOT_HANDLED,
    M_INPUT_ERROR
} MInputResultEnum;


typedef struct _MInputHostData
{
    MInputData buffer[2][MAKISE_GUI_INPUT_BUFFER_LEN]; //flipflop buffer. While first performing second will filling up

    uint8_t cur_buf : 1;
    
    uint32_t buf_index[2];

    MInputData (*result_handler)(MInputData d, MInputResultEnum res); //user can handle result of input and if it's required send one more input data which will be performed right now
    
#if MAKISE_GUI_INPUT_POINTER_ENABLE == 1
    uint8_t cursor_session : 2; // first bit - is session begun
                             // second - is cursor input allowed for this session
    MInputCursorEvent last;
#endif    

#if MAKISE_MUTEX
    MAKISE_MUTEX_t mutex;
#endif
    
} MInputHostData;
    
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
#if MAKISE_GUI_INPUT_POINTER_ENABLE == 1
void makise_gui_input_send_cursor(MHost *h, MInputEventEnum event, int32_t x, int32_t y, uint32_t time);
#endif
void makise_gui_input_send_char(MHost *h, char c);
/**
 * send all buffered events to gui
 *
 * @param h 
 * @return 
 */
void makise_gui_input_perform(MHost *h);

#ifdef __cplusplus
}
#endif

#endif
