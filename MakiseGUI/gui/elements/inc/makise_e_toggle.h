#ifndef MAKISE_E_TOGGLE_H
#define MAKISE_E_TOGGLE_H

#include "makise_config.h"

#if ( MAKISE_E_TOGGLE > 0 )

#ifdef __cplusplus
extern "C" {
#endif

#include "makise_e.h"

//Toogle - simple toogle. executes click() function when OK clicked.
typedef struct _MToogle MToogle;

typedef struct _MToogle {
    MakiseGUI*          gui;
    MElement            el;
    MakiseStyle*        style;

    char*               text;                       //text
    uint8_t             state;                      //state of toggle button. 1 if active, 0 if not.

    void                ( *toggled )    ( MToogle* b, uint8_t state ); //When state changed by input
    uint8_t             focus_state;
} MToggle;

void m_create_toggle ( MToggle*         b,
                       MContainer*      c,
                       MPosition        pos,
                       char*            text,
                       void             ( *toggled ) ( MToogle* b, uint8_t state ),
                       MakiseStyle*     style);

#ifdef __cplusplus
}
#endif

#endif

#endif
