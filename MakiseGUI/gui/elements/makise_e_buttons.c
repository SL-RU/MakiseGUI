#include "makise_e.h"

uint8_t _m_button_draw   (MElement* b);
MInputResultEnum _m_button_input  (MElement* b, MInputData data);
MFocusEnum _m_button_focus  (MElement* b, MFocusEnum act);

void m_create_button(MButton* b, MContainer *c,
		     int32_t x, int32_t y, uint32_t w, uint32_t h,
		     char* text,
		     void    (*click   )(MButton* b),
		     uint8_t (*onkey   )(MButton* b, MInputData data),
		     void    (*onfocus )(MButton* b, MFocusEnum type),
		     MakiseStyle *style)
{
    MElement *e = &b->el;
    e->gui = c->gui;

    e->data = b;

    e->draw = &_m_button_draw;
    e->predraw = 0;
    e->update = 0;
    e->input = &_m_button_input;
    e->focus = &_m_button_focus;
    e->is_parent = 0;
    
    e->position.x = x;
    e->position.y = y;
    e->position.width = w;
    e->position.height = h;

    e->enabled = 1;
    e->id = makise_g_newid();
    
    e->focus_prior = 1;
    
    b->text = text;

    b->click = click;
    b->onkey = onkey;
    b->onfocus = onfocus;
    
    b->style = style;
    
    makise_g_cont_add(c, e);
    
    printf("Button %d created\n", e->id);
}

uint8_t _m_button_draw   (MElement* b)
{
    MakiseStyleTheme *th = 0;

    if(((MButton*)b->data)->state == 0)
	th = &((MButton*)b->data)->style->normal;
    else if(((MButton*)b->data)->state == 1)
	th = &((MButton*)b->data)->style->focused;
    else if(((MButton*)b->data)->state >= 2)
    {
	th = &((MButton*)b->data)->style->active;
	((MButton*)b->data)->state --;
    }
    _m_e_helper_draw_box(b->gui->buffer, &b->position, th);
    
    makise_d_string(b->gui->buffer,
		    ((MButton*)b->data)->text, MDTextAll,
		    b->position.real_x + b->position.width / 2,
		    b->position.real_y + b->position.height / 2,
		    MDTextPlacement_Center,
		    ((MButton*)b->data)->style->font,
		    th->font_col);

    //printf("Button %d dr\n", b->id);
    return M_OK;
}

MInputResultEnum _m_button_input  (MElement* b, MInputData data)
{
    printf("but click %d %d\n", b->id, b->position.real_y);
    MButton *e = ((MButton*)b->data);
    if(e->onkey != 0)
	if(e->onkey(e, data) == M_INPUT_HANDLED)
	    return M_INPUT_HANDLED;

    if((data.key == M_KEY_OK
	|| data.key == M_KEY_CURSOR)
       && data.event == M_INPUT_CLICK &&
       e->click != 0)
    {
	e->click(e);
	e->state = 2;
	return M_INPUT_HANDLED;
    }
    
    return M_INPUT_NOT_HANDLED;
}
MFocusEnum _m_button_focus  (MElement* b, MFocusEnum act)
{
    MButton *e = ((MButton*)b->data);
    if(act & M_G_FOCUS_GET)
    {
	if(e->state != 1 && e->onfocus != 0)
	{
	    e->onfocus(e, M_G_FOCUS_GET);
	}
	((MButton*)b->data)->state = 1;
    }
    if(act == M_G_FOCUS_LEAVE)
    {
	if(e->state != 0 && e->onfocus != 0)
	{
	    e->onfocus(e, M_G_FOCUS_LEAVE);
	}
	((MButton*)b->data)->state = 0;
    }

    return (act == M_G_FOCUS_PREV || act == M_G_FOCUS_NEXT)
	? M_G_FOCUS_NOT_NEEDED
	: M_G_FOCUS_OK;
}

/* void m_button_set_click   (MButton *b, void (*click   )(MButton* b)) */
/* { */
/*     b->click = click; */
/* } */
/* void m_button_set_onkey(MButton *b, uint8_t (*onkey)(MButton* b, MInputData data)) */
/* { */
/*     b->onkey = onkey; */
/* } */
/* void m_button_set_onfocus (MButton *b, void (*onfocus )(MButton* b, MFocusEnum type)) */
/* { */
/*     b->onfocus = onfocus; */
/* } */
