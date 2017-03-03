#include "makise_e.h"

uint8_t _m_textfield_draw   (MElement* b);

void m_create_text_field(MTextField* b, MContainer *c,
			 int32_t x, int32_t y, uint32_t w, uint32_t h,
			 char* text,
			 MakiseStyle *style)
{
    MElement *e = &b->el;
    e->gui = c->gui;

    e->data = b;
    

    e->draw = &_m_textfield_draw;
    e->predraw = 0;
    e->update = 0;
    e->input = 0;
    e->focus = 0;
    e->free =  0;
	
    e->position.x = x;
    e->position.y = y;
    e->position.width = w;
    e->position.height = h;

    e->enabled = 1;
    e->id = makise_g_newid();
    e->focus_prior = 0;
    
    b->text = text;
    
    b->style = style;
    
    makise_g_cont_add(c, e);
    
    printf("Text field %d created\n", e->id);
}

uint8_t _m_textfield_draw   (MElement* b)
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
