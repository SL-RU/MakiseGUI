#ifndef _MAKISE_H_G_CONTAINER
#define _MAKISE_H_G_CONTAINER 1

typedef struct _MContainer MContainer;


#include "makise_gui.h"
#include "makise_gui_elements.h"

typedef struct _MContainer
{
    MakiseGUI *gui;
    MElement* el;
    MPosition *position;
    MElement* first;
    MElement* last;
    MElement* focused;
    
    void (*on_add)(MElement* el);
} MContainer;

void makise_g_cont_add(MContainer * cont, MElement *el);
void makise_g_cont_rem(MContainer * cont, MElement *el);

uint8_t makise_g_cont_call   (MContainer *cont, uint8_t type);
MInputResultEnum makise_g_cont_input  (MContainer *cont, MInputData data);
MFocusEnum makise_g_cont_focus_next(MContainer *cont);
MFocusEnum makise_g_cont_focus_prev(MContainer *cont);
MElement makise_g_cont_element_on_point(MContainer *cont, int32_t  x, int32_t y);
#endif
