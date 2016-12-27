#ifndef _MAKISE_H_G_CONTAINER
#define _MAKISE_H_G_CONTAINER 1

typedef struct _MContainer MContainer;

#include "makise_gui.h"


typedef struct _MContainer
{
    MElement* el;
    MElement* first;
    MElement* last;

    void (*on_add)(MElement* el);
} MContainer;

void makise_g_cont_add(MContainer * cont, MElement *el);
void makise_g_cont_rem(MContainer * cont, MElement *el);

#endif
