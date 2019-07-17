#ifndef MAKISE_E_TEXT_FIELD_H
#define MAKISE_E_TEXT_FIELD_H

#include "makise_config.h"

#if ( MAKISE_E_TEXT_FIELD > 0 )

#ifdef __cplusplus
extern "C" {
#endif

#include "makise_e.h"

typedef struct {
    const MakiseFont*       font;
    uint32_t                font_line_spacing;

    //scroll
    uint8_t                 scroll_width; //if == 0 then total line count won't be calculated
    MColor                scroll_bg_color; //may be 0
    MColor                scroll_color;

    
    MakiseStyleTheme  normal;
    MakiseStyleTheme  focused;
    MakiseStyleTheme  active;
} MakiseStyle_TextField;

    
// This one will display multiline text with multiline, folding and cutting support.
typedef struct {
    MakiseGUI*            gui;
    MElement              el;

    char*                 text;
    uint32_t              len;

    uint8_t               enable_scroll;
    uint32_t              current_line;
    uint32_t              total_lines ;

    uint8_t                state; //focus state
    MakiseStyle_TextField* style;
} MTextField;

void m_create_text_field( MTextField*   b,
                          MContainer*   c,
                          MPosition     pos,
                          char*         text,
			  uint32_t      len,
			  uint8_t       enable_scroll,
                          MakiseStyle_TextField*  style );

void m_text_field_set_text ( MTextField*   b,
			     char*         text,
			     uint32_t      len );
			     

#ifdef __cplusplus
}
#endif

#endif

#endif
