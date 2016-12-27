#ifndef _MAKISE_H_G
#define _MAKISE_H_G 1

#include "makise.h"
#include "makise_gui_elements.h"
#include "makise_gui_container.h"

//Root container for all elements
typedef struct 
{
    MContainer host;
    
    
} MHost;


uint32_t makise_gui_newid();
void makise_gui_host_draw(MHost *h);

#endif
