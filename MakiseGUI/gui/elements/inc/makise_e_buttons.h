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
} MakiseTheme_Button;


typedef struct {
    const MakiseFont* font;
    MakiseTheme_Button normal;
    MakiseTheme_Button focused;
    MakiseTheme_Button active;
} MakiseStyle_Button;

// Button - simple button. executes click() function when OK clicked.
typedef struct _MButton MButton;

typedef struct _MButton {
    MElement            el;

    char*               text;
    MakiseStyle_Button*  style;

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
                      MakiseStyle_Button*   style );

void m_button_set_text    (MButton *b, char *c);
void m_button_set_click   (MButton *b, void (*click   )(MButton* b));
void m_button_set_onkey   (MButton *b, uint8_t (*onkey)(MButton* b, MInputData data));
void m_button_set_onfocus (MButton *b, void (*onfocus )(MButton* b, MFocusEnum type));
    
#ifdef __cplusplus
}
#endif

#endif

#endif
