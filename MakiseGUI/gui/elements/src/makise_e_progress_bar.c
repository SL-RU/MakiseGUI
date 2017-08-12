#include "makise_e_progress_bar.h"

#if ( MAKISE_E_PROGRESS_BAR > 0 )

#ifdef __cplusplus
extern "C" {
#endif

static uint8_t draw    ( MElement* b );

static char name[] = "Progress bar";
void m_create_progress_bar ( MProgressBar*                  b,
                             MContainer*                    c,
                             MPosition                      pos,
                             uint8_t                        duty,
                             uint8_t                        margin,
                             MakiseProgressBarStyleTheme*   style ) {
    b->duty         = duty;
    b->style        = style;
    b->margin       = margin;

    MElement *e     = &b->el;
    m_element_create        ( e, ( c == 0 ) ? NULL : c->gui, name, b, 1, 1, pos, &draw, NULL, NULL, NULL, NULL, 0, NULL );
    makise_g_cont_add       ( c, e );

#if ( MAKISE_ENABLE_DEBUG_OUTPUT > 0 )
    MAKISE_DEBUG_OUTPUT("Progress_bar %d created.\n", e->id);
#endif
}

static uint8_t draw ( MElement* b ) {
    MProgressBar *e =        ((MProgressBar*)b->data);

    makise_d_rect_filled( b->gui->buffer,
                          b->position.real_x,   b->position.real_y,
                          e->el.position.width,
                          e->el.position.height,
                          e->style->border_color,
                          e->style->bg_color );

    makise_d_rect_filled( b->gui->buffer,
                          b->position.real_x + e->margin + 1,   b->position.real_y + e->margin + 1,
                          ( e->el.position.width - e->margin * 2 - 2 ) * e->duty / 100,
                          e->el.position.height - e->margin * 2 - 2,
                          e->style->duty_color,
                          e->style->duty_color );

    return 0;
}

void m_progress_bar_set_duty ( MProgressBar* b, uint8_t duty ) {
    if ( duty > 100 ) {
        b->duty = 100;
        return;
    } else {
        b->duty = duty;
    }
}


#ifdef __cplusplus
}
#endif

#endif
