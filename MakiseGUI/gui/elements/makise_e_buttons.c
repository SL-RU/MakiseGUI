#include "makise_e.h"

uint8_t _m_button_draw   (MElement* b);
uint8_t _m_button_predraw(MElement* b);
MInputResultEnum _m_button_input  (MElement* b, MInputData data);
MFocusEnum _m_button_focus  (MElement* b, MFocusEnum act);
uint8_t _m_button_free   (MElement* b);

void m_button_create(MButton* b, MContainer *c,
			 int32_t x, int32_t y, uint32_t w, uint32_t h,
			 char* text,
			 uint32_t bgcolor, uint32_t fontcolor, uint32_t bordercolor)
{
    MElement *e = &b->el;
    e->gui = c->gui;

    e->data = b;
    
    makise_g_cont_add(c, e);

    e->draw = &_m_button_draw;
    e->predraw = &_m_button_predraw;
    e->update = 0;
    e->input = &_m_button_input;
    e->focus = &_m_button_focus;
    e->free = &_m_button_free;

    e->position.x = x;
    e->position.y = y;
    e->position.width = w;
    e->position.height = h;

    e->enabled = 1;
    e->id = makise_g_newid();
    e->focus_prior = 1;
    
    b->text = text;
    b->bgcolor = bgcolor;
    b->fontcolor = fontcolor;
    b->bordercolor = bordercolor;

    printf("Button %d created\n", e->id);
}

uint8_t _m_button_draw   (MElement* b)
{
    makise_d_rect_filled(b->gui->buffer,
			 b->position.real_x,
			 b->position.real_y,
			 b->position.width,
			 b->position.height,
			 ((MButton*)b->data)->bordercolor,
			 ((MButton*)b->data)->bgcolor & ((MButton*)b->data)->state);
    
    makise_d_line(b->gui->buffer,
		  b->position.real_x,
		  b->position.real_y,
		  0, 0, 3);
    
    makise_d_string(b->gui->buffer,
		    ((MButton*)b->data)->text, MDTextLenAuto,
		    b->position.real_x + b->position.width / 2,
		    b->position.real_y + b->position.height / 2,
		    MDTextPlacement_Center,
		    &F_Arial24,
		    ((MButton*)b->data)->fontcolor);

    //printf("Button %d dr\n", b->id);
    return M_OK;
}
uint8_t _m_button_predraw(MElement* b)
{
//    printf("Button %d predr\n", b->id);
    return M_OK;
}
MInputResultEnum _m_button_input  (MElement* b, MInputData data)
{
    printf("but click %d %d\n", b->id, b->position.real_y);
    switch(data.key)
    {
    case M_KEY_UP    : b->position.y -= 1; return M_INPUT_HANDLED; break;
    case M_KEY_DOWN  : b->position.y += 1; return M_INPUT_HANDLED; break;
    case M_KEY_LEFT  : b->position.x -= 1; return M_INPUT_HANDLED; break;
    case M_KEY_RIGHT : b->position.x += 1; return M_INPUT_HANDLED; break;
    default: break;
    }
    
    return M_INPUT_NOT_HANDLED;
}
MFocusEnum _m_button_focus  (MElement* b, MFocusEnum act)
{
    if(act & M_G_FOCUS_GET)
	((MButton*)b->data)->state = 1;
    if(act == M_G_FOCUS_LEAVE)
	((MButton*)b->data)->state = 0;

    return (act == M_G_FOCUS_PREV || act == M_G_FOCUS_NEXT)
	? M_G_FOCUS_NOT_NEEDED
	: M_G_FOCUS_OK;
}
uint8_t _m_button_free   (MElement* b)
{
    return M_ERROR;
}
