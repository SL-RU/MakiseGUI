#ifndef MAKISE_E_BUTTONS_H
#define MAKISE_E_BUTTONS_H      1

#include "makise_config.h"

#if ( MAKISE_E_BUTTONS > 0 )

#ifdef __cplusplus
extern "C" {
#endif

#include "makise_e.h"

typedef struct {
    uint32_t bg_color;
    uint32_t font_col;
    uint32_t border_c;

    uint16_t double_border;
} MakiseButtonTheme;


typedef struct {
    const MakiseFont* font;
    MakiseButtonTheme normal;
    MakiseButtonTheme focused;
    MakiseButtonTheme active;
} MakiseButtonStyle;

// Button - simple button. executes click() function when OK clicked.
typedef struct _MButton MButton;

typedef struct _MButton {
    MakiseGUI*          gui;
    MElement            el;

    char*               text;
    MakiseButtonStyle*  style;

    void                ( *click )      ( MButton* b );                         // When OK button clicked.
    uint8_t             ( *onkey )      ( MButton* b, MInputData data );        // Handle any key pressed on button.
                                                                                // Return 1 if you want click method be executed if was click event.
                                                                                // 0 if not and send NOT_HANDLED.
                                                                                // 2 if not and send HANDLED result.
    void                ( *onfocus )    ( MButton* b, MFocusEnum type );        // type == M_G_FOCUS_GET when focus recieved and M_G_FOCUS_LEAVE when dropped.

    uint8_t state;
} MButton;

void m_create_button( MButton*              b,
                      MContainer*           c,
                      MPosition             pos,
                      char*                 text,
                      void                  ( *click )    ( MButton* b ),
                      uint8_t               ( *onkey )    ( MButton* b, MInputData data ),
                      void                  ( *onfocus )  ( MButton* b, MFocusEnum type ),
                      MakiseButtonStyle*    style );

#ifdef __cplusplus
}
#endif

#endif

#endif
