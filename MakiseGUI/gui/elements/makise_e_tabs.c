#include "makise_e.h"

uint8_t _m_tabs_draw   (MElement* b);
uint8_t _m_tabs_predraw(MElement* b);
MInputResultEnum _m_tabs_input  (MElement* b, MInputData data);
MFocusEnum _m_tabs_focus  (MElement* b, MFocusEnum act);

void _m_tabs_init_tabs(MTabs* b);

void m_create_tabs(MTabs* b, MContainer *c,
		   int32_t x, int32_t y, uint32_t w, uint32_t h,
		   MTabs_Tab *tabs,
		   uint32_t len,
		   MTabs_Type type,
		   uint32_t size,
		   MakiseStyle *style)
{
    MElement *e = &b->el;
    e->gui = c->gui;

    e->data = b;
    
    makise_g_cont_add(c, e);

    e->draw = &_m_tabs_draw;
    e->predraw = &_m_tabs_predraw;
    e->update = 0;
    e->input = &_m_tabs_input;
    e->focus = &_m_tabs_focus;
    e->free = 0;

    e->position.x = x;
    e->position.y = y;
    e->position.width = w;
    e->position.height = h;

    e->enabled = 1;
    e->id = makise_g_newid();
    e->focus_prior = 1;
    
    b->style = style;

    b->tabs = tabs;
    b->len = len;
    b->type = type;
    b->size = size;
    b->selected = 0;

    _m_tabs_init_tabs(b);

    if(type == MTabs_Type_Up)
    {
	b->container_pos.x = 0;
	b->container_pos.y = size;
	b->container_pos.height = h - size;
	b->container_pos.width = w;
    } if(type == MTabs_Type_Left)
    {
	b->container_pos.x = size;
	b->container_pos.y = 0;
	b->container_pos.height = h;
	b->container_pos.width = w - size;
    } if(type == MTabs_Type_None)
    {
	b->container_pos.x = 0;
	b->container_pos.y = 0;
	b->container_pos.height = h;
	b->container_pos.width = w;
    }
    
    printf("Tabs %d created\n", e->id);

}


uint8_t _m_tabs_draw   (MElement* b)
{
    MakiseStyleTheme *th = 0;

    if(((MTabs*)b->data)->state == 0)
	th = &((MTabs*)b->data)->style->normal;
    else if(((MTabs*)b->data)->state == 1)
	th = &((MTabs*)b->data)->style->focused;
    
    MTabs *t = ((MTabs*)b->data);
    
    makise_d_rect_filled(b->gui->buffer,
			 t->container_pos.real_x, t->container_pos.real_y,
			 t->container_pos.width, t->container_pos.height,
			 th->border_c,
			 th->bg_color);

    if(t->len == 0 || t->tabs == 0)
	return M_OK;
	
    if(t->selected >= t->len)
	t->selected = 0;

    if(t->type == MTabs_Type_Up)
    {
	uint32_t w = b->position.width / t->len;
	for (uint32_t i = 0; i < t->len; i++) {
	    makise_d_rect_filled(b->gui->buffer,
				 b->position.real_x + w * i, b->position.real_y,
				 w, t->size,
				 (t->selected != i) ?
				 t->style->unactive.border_c :
				 t->style->active.border_c,
				 (t->selected != i) ?
				 t->style->unactive.bg_color :
				 t->style->active.bg_color);
	    makise_d_string_frame(b->gui->buffer,
				  t->tabs[i].header , MDTextAll,
				  b->position.real_x + w * i + 3,
				  b->position.real_y, w - 6,
				  t->size,
				  t->style->font, 0,
				  (t->selected == i) ?
				  t->style->active.font_col :
				  t->style->unactive.font_col);
	}
    } else if(t->type == MTabs_Type_Left)
    {
	uint32_t h = b->position.height / t->len;
	for (uint32_t i = 0; i < t->len; i++) {
	    makise_d_rect_filled(b->gui->buffer,
				 b->position.real_x, b->position.real_y + h * i,
				 t->size, h,
				 (t->selected != i) ?
				 t->style->unactive.border_c :
				 t->style->active.border_c,
				 (t->selected != i) ?
				 t->style->unactive.bg_color :
				 t->style->active.bg_color);
	    makise_d_string_frame(b->gui->buffer,
				  t->tabs[i].header , MDTextAll,
				  b->position.real_x + 3, b->position.real_y + h * i,
				  t->size - 6, h,
				  t->style->font, 0,
				  (t->selected == i) ?
				  t->style->active.font_col :
				  t->style->unactive.font_col);
	}
    }
    return makise_g_cont_call(&t->tabs[t->selected].cont, M_G_CALL_DRAW);
//    return M_OK;
}
uint8_t _m_tabs_predraw(MElement* b)
{
    MTabs *t = ((MTabs*)b->data);

    if(t->len == 0 || t->tabs == 0)
	return M_OK;
	
    if(t->selected >= t->len)
	t->selected = 0;

    t->container_pos.real_y = t->container_pos.y + b->position.real_y;
    t->container_pos.real_x = t->container_pos.x + b->position.real_x;
    
    return makise_g_cont_call(&t->tabs[t->selected].cont, M_G_CALL_PREDRAW);
}
MInputResultEnum _m_tabs_input  (MElement* b, MInputData data)
{
    MTabs *t = ((MTabs*)b->data);

    if(t->len == 0 || t->tabs == 0)
	return M_INPUT_NOT_HANDLED;
	
    if(t->selected >= t->len)
	t->selected = 0;

//    if(t->state == 2) //for tab selection
//	return M_INPUT_NOT_HANDLED;

    if(data.key == M_KEY_TAB_NEXT)
    {
	if(t->selected < t->len - 1)
	    t->selected ++;
	else
	    t->selected = 0;
	return M_INPUT_HANDLED;
    } else if(data.key == M_KEY_TAB_BACK)
    {
	if(t->selected >= 1)
	    t->selected --;
	else
	    t->selected = t->len - 1;
	return M_INPUT_HANDLED;
    }
    
    if(t->tabs[t->selected].cont.focused == 0)
       makise_g_cont_focus_next(&t->tabs[t->selected].cont);
    if(t->tabs[t->selected].cont.focused != 0)
	return makise_g_cont_input(&t->tabs[t->selected].cont, data);
    return M_INPUT_NOT_HANDLED;
}
MFocusEnum _m_tabs_focus  (MElement* b, MFocusEnum act)
{
    MTabs *t = ((MTabs*)b->data);

    if(t->len == 0 || t->tabs == 0)
	return M_G_FOCUS_NOT_NEEDED;
	
    if(t->selected >= t->len)
	t->selected = 0;

//    if(t->state == 2) //for tab selection
//	return M_G_FOCUS_NOT_NEEDED;
    
    switch (act) {
    case M_G_FOCUS_GET:
	t->state = 1;
	makise_g_cont_focus_next(&t->tabs[t->selected].cont);
	return M_G_FOCUS_OK;
	break;
    case M_G_FOCUS_GET_NEXT:
	t->state = 1;
	makise_g_cont_focus_next(&t->tabs[t->selected].cont);
	return M_G_FOCUS_OK;
	break;
    case M_G_FOCUS_GET_PREV:
	t->state = 1;
	makise_g_cont_focus_prev(&t->tabs[t->selected].cont);
	return M_G_FOCUS_OK;
	break;
    case M_G_FOCUS_LEAVE:
	t->state = 0;
	makise_g_focus(t->tabs[t->selected].cont.focused, M_G_FOCUS_LEAVE);
	t->tabs[t->selected].cont.focused = 0;
	return M_G_FOCUS_OK;
	break;
    case M_G_FOCUS_NEXT:
	return makise_g_cont_focus_next(&t->tabs[t->selected].cont);
	break;
    case M_G_FOCUS_PREV:
	return makise_g_cont_focus_prev(&t->tabs[t->selected].cont);
	break;
    }
    
    return M_G_FOCUS_NOT_NEEDED;
}

void _m_tabs_init_tabs(MTabs* b)
{
    if(b->tabs == 0)
	return;
    for (uint32_t i = 0; i < b->len; i++) {
	b->tabs[i].cont.gui = b->el.gui;
	b->tabs[i].cont.el = &b->el;
	b->tabs[i].cont.first = 0;
	b->tabs[i].cont.last = 0;
	b->tabs[i].cont.focused = 0;
	b->tabs[i].cont.position = &b->container_pos;
    }
    b->container_pos = b->el.position;
}
