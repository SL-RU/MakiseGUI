#include "makise_e.h"

uint8_t _m_lable_draw   (MElement* b);

void m_create_lable(MLable* b, MContainer *c,
			 int32_t x, int32_t y, uint32_t w, uint32_t h,
			 char* text,
			 MakiseStyle *style)
{
    MElement *e = &b->el;
    e->gui = c->gui;

    e->data = b;
    
    makise_g_cont_add(c, e);

    e->draw = &_m_lable_draw;
    e->predraw = 0;
    e->update = 0;
    e->input = 0;
    e->focus = 0;
    e->is_parent =  0;
	
    e->position.x = x;
    e->position.y = y;
    e->position.width = w;
    e->position.height = h;

    e->enabled = 1;
    e->id = makise_g_newid();
    e->focus_prior = 0;
    
    b->text = text;
    
    b->style = style;
    
    printf("Lable %d created\n", e->id);
}

uint8_t _m_lable_draw   (MElement* b)
{
    MakiseStyleTheme *th = &((MLable*)b->data)->style->normal;
    
    _m_e_helper_draw_box(b->gui->buffer, &b->position, th);
    
    makise_d_string(b->gui->buffer,
		    ((MLable*)b->data)->text, MDTextAll,
		    b->position.real_x + 2,// + b->position.width / 2,
		    b->position.real_y,// + b->position.height / 2,
		    MDTextPlacement_LeftUp,
		    ((MLable*)b->data)->style->font,
		    th->font_col);
    
    return M_OK;
}
