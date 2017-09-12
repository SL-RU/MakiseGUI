#ifndef MAKISE_E_STATUS_BAR_H
#define MAKISE_E_STATUS_BAR_H       1

#include "makise_config.h"

#if ( MAKISE_E_STATUS_BAR > 0 )

#ifdef __cplusplus
extern "C" {
#endif

#include "makise_e.h"

typedef struct {
    const MakiseFont*       const font_ercent;
    const uint32_t                font_font_ercent_col;
    const uint32_t                bg_color;
    const uint32_t                border_color;
} MakiseStyle_SMPlayerStatusBar;

typedef struct {
    uint32_t ( *get_state_play )          ( void );
    uint32_t ( *get_percent_battery )     ( void );
} MPlayerStatusBar_CallbackFunc;

typedef struct {
    MElement                        e;
    MPlayerStatusBar_CallbackFunc*  f;
    MakiseStyle_SMPlayerStatusBar*  s;
} MPlayerStatusBar;

void m_create_player_status_bar( MPlayerStatusBar*              b,
                                 MakiseGUI*                     gui,
                                 MPosition                      pos,
                                 MakiseStyle_SMPlayerStatusBar* s,
                                 MPlayerStatusBar_CallbackFunc* f );

void m_player_status_bar_add_to_container( MContainer*         c,
                                           MPlayerStatusBar*   b );

#ifdef __cplusplus
}
#endif

#endif

#endif
