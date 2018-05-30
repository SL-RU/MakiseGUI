#include "makise_e.h"

void _m_e_helper_draw_box( MakiseBuffer* b, MPosition *p, const MakiseStyleTheme *th ) {
    _m_e_helper_draw_box_param( b, p, th->border_c, th->bg_color, th->thickness );
}

void _m_e_helper_draw_box_param ( MakiseBuffer* b, MPosition *p,
                                  MColor border_color, MColor bg_color, uint16_t thickness ) {
    makise_d_rect_filled( b,
                          (MPoint){p->real_x,
                                  p->real_y},
                          p->width,
                          p->height,
                          thickness,
                          border_color,
                          bg_color);
}
