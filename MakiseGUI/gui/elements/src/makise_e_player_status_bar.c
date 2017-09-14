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

uint8_t play_ar[] = { 0x06, 0x1e, 0x3e, 0x7e, 0x7e, 0x3e, 0x1e, 0x06, 0x00 };
const MakiseBitmap play_bitmap = {
    8,
    8,
    play_ar
};

uint8_t pause_ar[] = { 0xff, 0x81, 0xa5, 0xa5, 0xa5, 0xa5, 0x81, 0xff, 0x00 };
const MakiseBitmap pause = {
    8,
    8,
 pause_ar
};

uint8_t stop_ar[] = { 0xff, 0x81, 0xbd, 0xbd, 0xbd, 0xbd, 0x81, 0xff, 0x00,  };
const MakiseBitmap stop = {
    8,
    8,
    stop_ar
};


static uint8_t draw ( MElement* b ) {
    MPlayerStatusBar *p = ( MPlayerStatusBar* )b->data;
    _m_e_helper_draw_box_param( b->gui->buffer, &b->position, p->s->border_color, p->s->bg_color, 0 );


    // Play.

    const MakiseBitmap* state_bm;
    uint32_t state = p->f->get_state_play();
    switch ( state ) {
    case 0: state_bm = &stop;           break;
    case 1: state_bm = &play_bitmap;    break;
    case 2: state_bm = &pause;          break;
    }

    makise_d_bitmap( b->gui->buffer, 2, 2, state_bm, MC_Black );

    // Bat.
    uint32_t bat = p->f->get_percent_battery();
    char string_bat[10];
    convert_uint32_t_to_string( bat, string_bat );

    uint32_t x_text_bat = b->position.real_x + b->position.width - 1;

    uint32_t w_bat = makise_d_string_width( string_bat, 3, p->s->font_ercent );
    uint32_t w_pr  = makise_d_string_width( "%", 1, p->s->font_ercent );

    x_text_bat -= w_bat + w_pr;
    x_text_bat -= 20;

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

    x_text_bat += w_pr;
    makise_d_rect_filled( b->gui->buffer,
                          x_text_bat,
                          b->position.real_y + 2,
                          16,
                          8,
                          MC_Black,
                          MC_White );

    uint32_t width_bat = (float)16 / (float)100 *(float) bat;
    makise_d_rect_filled( b->gui->buffer,
                          x_text_bat,
                          b->position.real_y + 2,
                          width_bat,
                          8,
                          MC_Black,
                          MC_Black );

    makise_d_rect_filled( b->gui->buffer,
                          x_text_bat + 15,
                          b->position.real_y + 2 + 2,
                          4,
                          4,
                          MC_Black,
                          MC_White );

}

#ifdef __cplusplus
}
#endif

#endif

