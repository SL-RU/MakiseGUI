#ifndef MAKISE_E_LABLE_H
#define MAKISE_E_LABLE_H

#include "makise_config.h"

#if ( MAKISE_E_LABLE > 0 )

#ifdef __cplusplus
extern "C" {
#endif

#include "makise_e.h"

// Lable.
// Simply draws required text.
// It can be only in one line. And it won't be cutting and folding text.
typedef struct {
    MakiseGUI *gui;
    MElement el;

    char *text;

    MakiseStyle* style;
} MLable;

void m_create_lable( MLable*        b,
                     MContainer*    c,
                     MPosition      pos,
                     char*          text,
                     MakiseStyle*   style );

#ifdef __cplusplus
}
#endif

#endif

#endif
