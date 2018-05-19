#include "makise_e.h"

void _m_e_helper_draw_box( MakiseBuffer* b, MPosition *p, const MakiseStyleTheme *th ) {
    _m_e_helper_draw_box_param( b, p, th->border_c, th->bg_color, th->double_border );
}

void _m_e_helper_draw_box_param ( MakiseBuffer* b, MPosition *p, uint32_t border_color, uint32_t bg_color, uint16_t double_border ) {
    makise_d_rect_filled( b,
                          p->real_x,
                          p->real_y,
                          p->width,
                          p->height,
                          border_color,
                          bg_color);

    if ( double_border )
        makise_d_rect( b,
                       p->real_x + 2,
                       p->real_y + 2,
                       p->width  - 4,
                       p->height - 4,
                       border_color );
}
