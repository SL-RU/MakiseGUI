#include "makise_e_text_field.h"

#if ( MAKISE_E_TEXT_FIELD > 0 )

#ifdef __cplusplus
extern "C" {
#endif

static uint8_t draw   (MElement* b);

static char name[] = "Text field";
void m_create_text_field(MTextField* b, MContainer *c,
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
    
    MAKISE_DEBUG_OUTPUT("Text field %d created\n", e->id);
}

uint8_t draw   (MElement* b)
{
    MakiseStyleTheme *th = &((MTextField*)b->data)->style->normal;
    
    _m_e_helper_draw_box(b->gui->buffer, &b->position, th);
    
    makise_d_string_frame(b->gui->buffer,
			  ((MTextField*)b->data)->text, MDTextAll,
			  b->position.real_x + 2,// + b->position.width / 2,
			  b->position.real_y,// + b->position.height / 2,
			  b->position.width - 4,
			  b->position.height,
//		    MDTextPlacement_LeftUp,
			  ((MTextField*)b->data)->style->font,
			  ((MTextField*)b->data)->style->font_line_spacing,
			  th->font_col);
    
    return M_OK;
}

#ifdef __cplusplus
}
#endif

#endif
