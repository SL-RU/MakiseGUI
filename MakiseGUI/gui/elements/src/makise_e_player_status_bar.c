#include "makise_e_player_status_bar.h"

#if ( MAKISE_E_STATUS_BAR > 0 )

#ifdef __cplusplus
extern "C" {
#endif

static uint8_t draw ( MElement* b );

static char *name = "PlayerStatusBar";

void m_create_player_status_bar( MPlayerStatusBar*              b,
                                 MContainer*                    c,
                                 MPosition                      pos,
                                 MakiseStyle_SMPlayerStatusBar* s,
                                 MPlayerStatusBar_CallbackFunc* f ) {
    MElement *e = &b->e;
    m_element_create(e, c->gui, name, b,
             1, 1, pos,
             &draw,
             0,
             0,
             NULL,
             NULL,
             0, 0);

    b->s = s;
    b->f = f;

    makise_g_cont_add( c, e );

#if ( MAKISE_ENABLE_DEBUG_OUTPUT > 0 )
    MAKISE_DEBUG_OUTPUT( "PlayerStatusBar %d created\n", e->id );
#endif
}

static uint8_t draw ( MElement* b ) {
    MPlayerStatusBar *p = ( MPlayerStatusBar* )b->data;
    _m_e_helper_draw_box_param( b->gui->buffer, &b->position, p->s->border_color, p->s->bg_color, 0 );
    uint32_t bat = p->f->get_percent_battery();
    char string_bat[10];
    convert_uint32_t_to_string( bat, string_bat );

    uint32_t x_text_bat = b->position.real_x + b->position.width - 1;

    uint32_t w_bat = makise_d_string_width( string_bat, 3, p->s->font_ercent );
    uint32_t w_pr  = makise_d_string_width( "%", 1, p->s->font_ercent );

    x_text_bat -= w_bat + w_pr;

    makise_d_string( b->gui->buffer, string_bat, 10,
                     x_text_bat, 2,
                     MDTextPlacement_LeftUp,
                     p->s->font_ercent, p->s->font_font_ercent_col );

    x_text_bat += w_bat;

    makise_d_string( b->gui->buffer, "%", 1,
                     x_text_bat,
                     2,
                     MDTextPlacement_LeftUp,
                     p->s->font_ercent, p->s->font_font_ercent_col );

}

#ifdef __cplusplus
}
#endif

#endif

