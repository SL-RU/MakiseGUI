#include "makise_e.h"

void _m_e_helper_draw_box(MakiseBuffer* b, MPosition *p, MakiseStyleTheme *th)
{
    makise_d_rect_filled(b,
			 p->real_x,
			 p->real_y,
			 p->width,
			 p->height,
			 th->border_c,
			 th->bg_color);

    if(th->double_border)
	makise_d_rect(b,
		      p->real_x+2,
		      p->real_y+2,
		      p->width-4,
		      p->height-4,
		      th->border_c);
}
