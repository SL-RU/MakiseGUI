#include "makise_e.h"

uint8_t _m_canvas_draw   (MElement* b);
uint8_t _m_canvas_predraw(MElement* b);
MInputResultEnum _m_canvas_input  (MElement* b, MInputData data);
MFocusEnum _m_canvas_focus  (MElement* b, MFocusEnum act);

char _m_canvas_name[] = "Canvas";
void m_create_canvas(MCanvas* b, MContainer *c,
		     MPosition pos,
		     MakiseStyle *style)
{
    MElement *e = &b->el;
    e->gui = c->gui;

    e->name = _m_canvas_name;
    
    e->data = b;
    
    makise_g_cont_add(c, e);

    e->draw = &_m_canvas_draw;
    e->predraw = &_m_canvas_predraw;
    e->update = 0;
    e->input = &_m_canvas_input;
    e->focus = &_m_canvas_focus;
    e->is_parent = 1;
    e->children = &b->cont;

    e->position = pos;

    e->enabled = 1;
    e->id = makise_g_newid();
    e->focus_prior = 1;
    
    b->style = style;

    b->cont.gui = c->gui;
    b->cont.el  = e;
    b->cont.position = &e->position;

    printf("Button %d created\n", e->id);

}


uint8_t _m_canvas_draw   (MElement* b)
{
    MakiseStyleTheme *th = 0;

    if(((MCanvas*)b->data)->state == 0)
	th = &((MCanvas*)b->data)->style->normal;
    else if(((MCanvas*)b->data)->state == 1)
	th = &((MCanvas*)b->data)->style->focused;
    
    _m_e_helper_draw_box(b->gui->buffer, &b->position, th);


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
	return makise_g_cont_input(&((MCanvas*)b->data)->cont, data);
    return M_INPUT_NOT_HANDLED;
}
MFocusEnum _m_canvas_focus  (MElement* b, MFocusEnum act)
{
    switch (act) {
    case M_G_FOCUS_GET:
	((MCanvas*)b->data)->state = 1;
	return makise_g_cont_focus_next(&((MCanvas*)b->data)->cont);
	return M_G_FOCUS_OK;
	break;
    case M_G_FOCUS_GET_NEXT:
	((MCanvas*)b->data)->state = 1;
	return makise_g_cont_focus_next(&((MCanvas*)b->data)->cont);
	//return M_G_FOCUS_OK;
	break;
    case M_G_FOCUS_GET_PREV:
	((MCanvas*)b->data)->state = 1;
	return makise_g_cont_focus_prev(&((MCanvas*)b->data)->cont);
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
    default: break;
    }
    
    return M_G_FOCUS_NOT_NEEDED;
}
