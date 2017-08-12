#ifndef MAKISE_E_PROGRESS_BAR_H
#define MAKISE_E_PROGRESS_BAR_H     1

#include "makise_config.h"

#if ( MAKISE_E_LABLE > 0 )

#ifdef __cplusplus
extern "C" {
#endif

#include "makise_e.h"

typedef struct {
    uint32_t    bg_color;
    uint32_t    border_color;
    uint32_t    duty_color;
} MakiseProgressBarStyleTheme;

// Progress bar.
typedef struct {
    MakiseGUI*                      gui;
    MElement                        el;

    uint8_t                         duty;
    uint8_t                         margin;

    MakiseProgressBarStyleTheme*    style;
} MProgressBar;

void m_create_progress_bar ( MProgressBar*                  b,
                             MContainer*                    c,
                             MPosition                      pos,
                             uint8_t                        duty,           // Duty factor [ 0..100 ].
                             uint8_t                        margin,
                             MakiseProgressBarStyleTheme*   style );

void m_progress_bar_set_duty ( MProgressBar* b, uint8_t duty );

#ifdef __cplusplus
}
#endif

#endif

#endif
