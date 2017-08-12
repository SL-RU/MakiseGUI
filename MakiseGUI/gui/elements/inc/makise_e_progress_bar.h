#ifndef MAKISE_E_PROGRESS_BAR_H
#define MAKISE_E_PROGRESS_BAR_H     1

#include "makise_config.h"

#if ( MAKISE_E_LABLE > 0 )

#ifdef __cplusplus
extern "C" {
#endif

#include "makise_e.h"

// Progress bar.
typedef struct {
    MakiseGUI*      gui;
    MElement        el;

    uint8_t         duty;
    MakiseStyle*    style;
} MProgressBar;

void m_create_progress_bar ( MProgressBar*  b,
                             MContainer*    c,
                             MPosition      pos,
                             uint8_t        duty,           // Duty factor [ 0..100 ].
                             MakiseStyle*   style );

#ifdef __cplusplus
}
#endif

#endif

#endif
