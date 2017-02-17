#include "makise_gui_container.h"

void makise_g_cont_add(MContainer * cont, MElement *el)
{
    el->parent = cont;
    if(cont->first == 0)
    {
	cont->first = el;
	cont->last = el;
	cont->focused = 0;
	el->next = 0;
	el->prev = 0;
	
	return;
    }
    MElement *m = cont->last;
    if(m == 0) //if last accidently didn't setted.
    {
	m = cont->first;
	while (m != 0)
	{
	    if(m->next == 0)
	    {
	    
		break;
	    }
	    else
		m = m->next;
	}
    }
    if(m != 0)
    {
	m->next = el;
	el->prev = m;
	el->next = 0;
	cont->last = el;
    }
}
void makise_g_cont_rem(MContainer * cont, MElement *el)
{
}

uint8_t makise_g_cont_call_common_predraw(MElement *b)
{
    if(b->parent != 0 && b->parent->position != 0)
    {
	b->position.real_x = b->position.x + b->parent->position->real_x;
	b->position.real_y = b->position.y + b->parent->position->real_y;
    }
    else
    {
	b->position.real_x = b->position.x;
	b->position.real_y = b->position.y;
    }
}
uint8_t makise_g_cont_call   (MContainer *cont, uint8_t type)
{
    if(cont == 0)
	return M_ZERO_POINTER;
    if(cont->first == 0)
	return M_ZERO_POINTER;

    if(cont->el != 0) //container itself
    {
	if(type == M_G_CALL_PREDRAW)
	    makise_g_cont_call_common_predraw(cont->el);
//	m_element_call(cont->el, type);
    }
    
    MElement *e = cont->first;
    uint8_t r = 0;
    while(e != 0)
    {
	if(type == M_G_CALL_PREDRAW)
	    makise_g_cont_call_common_predraw(e);
	r = m_element_call(e, type);
	e = e->next;
    }
    return M_OK;
}

MInputResultEnum makise_g_cont_input  (MContainer *cont, MInputData data)
{
    if(cont == 0)
	return M_ZERO_POINTER;

    if(cont->focused != 0)
	return cont->focused->input(cont->focused, data);

    return M_INPUT_NOT_HANDLED;
}

MFocusEnum makise_g_cont_focus_next(MContainer *cont)
{
    if(cont == 0)
	return M_ZERO_POINTER;

    if(cont->focused != 0)
    {
	if(cont->focused->focus != 0 && cont->focused->focus(cont->focused, M_G_FOCUS_NEXT) != M_G_FOCUS_OK)
	{
	    //if focused element didn't switched focus by itself or focus function pointer equals to zero, then let's try to do it by ourself.
	    MElement *e = cont->focused->next;
	    while(e != 0)
	    {
		if(e->focus_prior != 0)
		{
		    if(makise_g_focus(e, M_G_FOCUS_GET_NEXT) == M_G_FOCUS_OK)
			return M_G_FOCUS_OK;
		}
		e = e->next;
	    }

	    //if isn't no more elements requiring focus
	    if(cont->el == 0) //if cont is host container
	    {
		e = cont->first;
		while(e != 0)
		{
		    if(e->focus_prior != 0)
		    {
			if(makise_g_focus(e, M_G_FOCUS_GET_NEXT) == M_G_FOCUS_OK)
			    return M_G_FOCUS_OK;
		    }
		    e = e->next;
		}
	    }
	    return M_G_FOCUS_NOT_NEEDED;
	}
	else
	    return M_G_FOCUS_OK;
    }
    else
    {
	MElement *e = cont->first;
	while(e != 0)
	{
	    if(e->focus_prior != 0)
	    {
		if(makise_g_focus(e, M_G_FOCUS_GET_NEXT) == M_G_FOCUS_OK)
		{
//		    makise_g_focus(e, M_G_FOCUS_NEXT);
		    return M_G_FOCUS_OK;
		}
	    }
	    e = e->next;
	}
	return M_G_FOCUS_NOT_NEEDED;
    }
}
    
MFocusEnum makise_g_cont_focus_prev(MContainer *cont)
{
    if(cont == 0)
	return M_ZERO_POINTER;

    if(cont->focused != 0)
    {
	if(cont->focused->focus != 0 && cont->focused->focus(cont->focused, M_G_FOCUS_PREV) != M_G_FOCUS_OK)
	{
	    //if focused element didn't switched focus by itself or focus function pointer equals to zero, then let's try to do it by ourself.
	    MElement *e = cont->focused->prev;
	    while(e != 0)
	    {
		if(e->focus_prior != 0)
		{
		    if(makise_g_focus(e, M_G_FOCUS_GET_PREV) == M_G_FOCUS_OK)
		    {
			return M_G_FOCUS_OK;
		    }
		}
		e = e->prev;
	    }

	    //if isn't no more elements requiring focus
	    if(cont->el == 0) //if cont is host container
	    {
		e = cont->last;
		while(e != 0)
		{
		    if(e->focus_prior != 0)
		    {
			if(makise_g_focus(e, M_G_FOCUS_GET_PREV) == M_G_FOCUS_OK)
			    return M_G_FOCUS_OK;
		    }
		    e = e->prev;
		}
	    }
	    return M_G_FOCUS_NOT_NEEDED;
	}
	else
	    return M_G_FOCUS_OK;
    }
    else
    {
	MElement *e = cont->last;
	while(e != 0)
	{
	    if(e->focus_prior != 0)
	    {
		if(makise_g_focus(e, M_G_FOCUS_GET_PREV) == M_G_FOCUS_OK)
		{
//		    makise_g_focus(e, M_G_FOCUS_PREV);
		    return M_G_FOCUS_OK;
		}
	    }
	    e = e->prev;
	}
	return M_G_FOCUS_NOT_NEEDED;
    }
}
