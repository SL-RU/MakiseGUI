#ifndef _MAKISE_H_G_Input
#define _MAKISE_H_G_Input 1

#include <stdint.h>

typedef enum
{
    M_KEY_OK         = 1   , //main select button
    M_KEY_LEFT             , //cursor move
    M_KEY_RIGHT            , //cursor move
    M_KEY_UP               , //cursor move
    M_KEY_DOWN             , //cursor move
    M_KEY_ABOUT            , //view some info
    M_KEY_PROPERTY         , //change or view some properties
    M_KEY_CHARACTER        , //this is an character input
    M_KEY_TAB_NEXT         , //choose the next tab if possible
    M_KEY_TAB_BACK         , //choose the previous tab if possible
    M_KEY_USER_0     = 100 , //this and all the next keys can be used and defined by user
} MInputKeyEnum;

typedef enum
{
    M_INPUT_CLICK              = 1, //when key released
    M_INPUT_PRESSING              , //when key is currently pressing
} MInputEventEnum;

typedef struct
{
    MInputKeyEnum key;     //what is an event key 
    MInputEventEnum event; //what type of event is it
    uint32_t time;         //how log key was pressed or is pressing
    char c;                //what character was inputed if key == M_KEY_CHARACTER
} MInputData;

typedef enum
{
    M_INPUT_HANDLED,
    M_INPUT_NOT_HANDLED
} MInputResultEnum;

#include "makise_gui.h"

void makise_gui_input_send(MHost *h, MInputData d);
void makise_gui_input_send_button(MHost *h, MInputKeyEnum key, MInputEventEnum event, uint32_t time);

void makise_gui_input_perform(MHost *h);

#endif
