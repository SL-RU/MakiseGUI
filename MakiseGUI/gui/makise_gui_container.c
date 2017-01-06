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
    if(b->parent != 0 && b->parent->el != 0)
    {
	b->position.real_x = b->position.x + b->parent->el->position.x;
	b->position.real_x = b->position.x + b->parent->el->position.x;
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
	m_element_call(cont->el, type);
    }
    
    MElement *e = cont->first;
    uint8_t r = 0;
    while(1)
    {
	if(type == M_G_CALL_PREDRAW)
	    makise_g_cont_call_common_predraw(e);
	r = m_element_call(e, type);
	if(e->next == 0)
	    break;
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
