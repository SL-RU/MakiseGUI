#include "makise_e_progress_bar.h"

#if ( MAKISE_E_PROGRESS_BAR > 0 )

#ifdef __cplusplus
extern "C" {
#endif

static uint8_t draw    ( MElement* b );
static uint8_t predraw ( MElement* b );
static uint8_t update  ( MElement* b );
static MInputResultEnum input    ( MElement* el, MInputData data );

static char name[] = "Progress bar";
void m_create_progress_bar ( MProgressBar*  b,
                             MContainer*    c,
                             MPosition      pos,
                             uint8_t        duty,
                             MakiseStyle*   style ) {
    b->duty         = duty;
    b->style        = style;

    MElement *e     = &b->el;
    m_element_create        ( e, ( c == 0 ) ? NULL : c->gui, name, b, 1, 1, pos, &draw, NULL, &update, NULL, NULL, 0, NULL );
    makise_g_cont_add       ( c, e );

#if ( MAKISE_ENABLE_DEBUG_OUTPUT > 0 )
    MAKISE_DEBUG_OUTPUT("Progress_bar %d created.\n", e->id);
#endif
}

static uint8_t draw ( MElement* b ) {
    MButton *e =        ((MButton*)b->data);
    makise_d_rect_filled( b->gui->buffer,
                          b->position.real_x,   b->position.real_y,
                          b->gui->buffer->width,
                          b->gui->buffer->height,
                          e->style->normal.border_c,
                          e->style->normal.bg_color );

    //makise_d_rect( );

    return 0;
}

static uint8_t update ( MElement* b ) {
    ( void )b;
    return 0;
}

#ifdef __cplusplus
}
#endif

#endif
