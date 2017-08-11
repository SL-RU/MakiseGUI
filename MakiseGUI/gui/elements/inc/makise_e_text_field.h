#ifndef MAKISE_E_TEXT_FIELD_H
#define MAKISE_E_TEXT_FIELD_H

#include "makise_config.h"

#if ( MAKISE_E_TEXT_FIELD > 0 )

#ifdef __cplusplus
extern "C" {
#endif

#include "makise_e.h"

// This one will display multiline text with multiline, folding and cutting support.
typedef struct {
    MakiseGUI*      gui;
    MElement        el;

    char*           text;

    MakiseStyle*    style;
} MTextField;

void m_create_text_field( MTextField*   b,
                          MContainer*   c,
                          MPosition     pos,
                          char*         text,
                          MakiseStyle*  style );

#ifdef __cplusplus
}
#endif

#endif

#endif
