#include "makise_e_lable.h"

#if ( MAKISE_E_LABLE > 0 )

#ifdef __cplusplus
extern "C" {
#endif

static uint8_t draw   (MElement* b);

static char name[] = "Lable";
void m_create_lable(MLable* b, MContainer *c,
			 MPosition pos,
			 char* text,
			 MakiseStyle *style)
{
    MElement *e = &b->el;

    m_element_create(e, (c == 0) ? 0 : c->gui, name, b,
		     1, 1, pos,
		     &draw,
		     0,
		     0,
		     0,
		     0,
		     0, 0);
    
    b->text = text;
    b->style = style;

    makise_g_cont_add(c, e);
    
    printf("Lable %d created\n", e->id);
}

static uint8_t draw   (MElement* b)
{
    MakiseStyleTheme *th = &((MLable*)b->data)->style->normal;
    
    _m_e_helper_draw_box(b->gui->buffer, &b->position, th);
    if(((MLable*)b->data)->text != 0)
	makise_d_string(b->gui->buffer,
			((MLable*)b->data)->text, MDTextAll,
			b->position.real_x + 2,// + b->position.width / 2,
			b->position.real_y,// + b->position.height / 2,
			MDTextPlacement_LeftUp,
			((MLable*)b->data)->style->font,
			th->font_col);
    
    return M_OK;
}

#ifdef __cplusplus
}
#endif

#endif
