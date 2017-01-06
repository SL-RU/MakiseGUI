#ifndef _MAKISE_H_G
#define _MAKISE_H_G 1

#define M_G_CALL_DRAW         1
#define M_G_CALL_PREDRAW      2
#define M_G_CALL_UPDATE       3


typedef enum
{
    M_G_FOCUS_OK         = 1,
    M_G_FOCUS_GET        = 2,
    M_G_FOCUS_LEAVE      = 3,
    M_G_FOCUS_NOT_NEEDED = 4,
    M_G_FOCUS_ERROR      = 5,
} MFocusEnum;

typedef struct _MHost MHost;

#include "makise.h"
#include "makise_gui_input.h"
#include "makise_gui_elements.h"
#include "makise_gui_container.h"

//Root container for all elements
typedef struct _MHost
{
     MContainer *host;    
    
} MHost;


uint32_t makise_g_newid();
MFocusEnum makise_g_focus(MElement *el);

uint8_t makise_g_host_call   (MHost *host, uint8_t type);
MInputResultEnum makise_g_host_input  (MHost *host, MInputData data);
#endif
