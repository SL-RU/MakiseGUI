#include "makise_e.h"

uint8_t _m_canvas_draw   (MElement* b);
uint8_t _m_canvas_predraw(MElement* b);
MInputResultEnum _m_canvas_input  (MElement* b, MInputData data);
MFocusEnum _m_canvas_focus  (MElement* b, MFocusEnum act);
uint8_t _m_canvas_free   (MElement* b);


void m_canvas_create(MCanvas* b, MContainer *c,
		     int32_t x, int32_t y, uint32_t w, uint32_t h,
		     uint32_t bgcolor, uint32_t bordercolor)
{
    MElement *e = &b->el;
    e->gui = c->gui;

    e->data = b;
    
    makise_g_cont_add(c, e);

    e->draw = &_m_canvas_draw;
    e->predraw = &_m_canvas_predraw;
    e->update = 0;
    e->input = &_m_canvas_input;
    e->focus = &_m_canvas_focus;
    e->free = &_m_canvas_free;

    e->position.x = x;
    e->position.y = y;
    e->position.width = w;
    e->position.height = h;

    e->enabled = 1;
    e->id = makise_g_newid();
    e->focus_prior = 1;
    
    b->bgcolor = bgcolor;
    b->bordercolor = bordercolor;

    b->cont.gui = c->gui;
    b->cont.el  = e;

    printf("Button %d created\n", e->id);

}


uint8_t _m_canvas_draw   (MElement* b)
{
    makise_d_rect_filled(b->gui->buffer,
			 b->position.real_x,
			 b->position.real_y,
			 b->position.width,
			 b->position.height,
			 ((MCanvas*)b->data)->bordercolor,
			 ((MCanvas*)b->data)->bgcolor);

    if(((MCanvas*)b->data)->state)
	makise_d_rect(b->gui->buffer,
		      b->position.real_x+2,
		      b->position.real_y+2,
		      b->position.width-4,
		      b->position.height-4,
		      ((MCanvas*)b->data)->bordercolor);
	    

    
    makise_d_line(b->gui->buffer,
		  b->position.real_x,
		  b->position.real_y,
		  0, 0, 2);

    //printf("Canvas %d dr\n", b->id);
    return makise_g_cont_call(&((MCanvas*)b->data)->cont, M_G_CALL_DRAW);
//    return M_OK;
}
uint8_t _m_canvas_predraw(MElement* b)
{
    
    return makise_g_cont_call(&((MCanvas*)b->data)->cont, M_G_CALL_PREDRAW);
}
MInputResultEnum _m_canvas_input  (MElement* b, MInputData data)
{
    if(((MCanvas*)b->data)->cont.focused == 0)
	makise_g_cont_focus_next(&(((MCanvas*)b->data)->cont));
    if(((MCanvas*)b->data)->cont.focused != 0)
	return m_element_input(((MCanvas*)b->data)->cont.focused, data);
    return M_INPUT_NOT_HANDLED;
}
MFocusEnum _m_canvas_focus  (MElement* b, MFocusEnum act)
{
    switch (act) {
    case M_G_FOCUS_GET:
	((MCanvas*)b->data)->state = 1;
	makise_g_cont_focus_next(&((MCanvas*)b->data)->cont);
	return M_G_FOCUS_OK;
	break;
    case M_G_FOCUS_GET_NEXT:
	((MCanvas*)b->data)->state = 1;
	makise_g_cont_focus_next(&((MCanvas*)b->data)->cont);
	return M_G_FOCUS_OK;
	break;
    case M_G_FOCUS_GET_PREV:
	((MCanvas*)b->data)->state = 1;
	makise_g_cont_focus_prev(&((MCanvas*)b->data)->cont);
	return M_G_FOCUS_OK;
	break;
    case M_G_FOCUS_LEAVE:
	((MCanvas*)b->data)->state = 0;
	makise_g_focus(((MCanvas*)b->data)->cont.focused, M_G_FOCUS_LEAVE);
	((MCanvas*)b->data)->cont.focused = 0;
	return M_G_FOCUS_OK;
	break;
    case M_G_FOCUS_NEXT:
	return makise_g_cont_focus_next(&(((MCanvas*)b->data)->cont));
	break;
    case M_G_FOCUS_PREV:
	return makise_g_cont_focus_prev(&(((MCanvas*)b->data)->cont));
	break;
    }
    
    return M_G_FOCUS_NOT_NEEDED;
}
uint8_t _m_canvas_free   (MElement* b)
{
}
