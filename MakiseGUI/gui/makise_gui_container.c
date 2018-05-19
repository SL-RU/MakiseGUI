#include "makise_gui_container.h"

void makise_g_cont_init(MContainer *c)
{
    if(c == 0)
        return;
    
    //clear structure
    memset(c, 0, sizeof(MContainer));

    c->first = 0;
    c->last = 0;
    c->position = 0;
    c->focused = 0;
    c->el = 0;
    c->host = 0;
    c->isolated = MContainer_NotIsolated;
}

/**
 * Updates host in all children's containers and MElements
 *
 * @param container container
 * @param host host
 * @return 
 */
void _makise_g_cont_update_host(MContainer * cont, MHost *host)
{
    cont->host = host;
    
    MElement *e = cont->first;
    while (e != 0) {
	e->host = host;
	if(e->is_parent) {
	    _makise_g_cont_update_host(e->children, host);
	}
	e = e->next;
    }
}
void makise_g_cont_add(MContainer * cont, MElement *el)
{
    if(cont == 0 || el == 0)
	return;
    el->parent = cont;
    el->host = cont->host;
    if(el->is_parent) {
	_makise_g_cont_update_host(el->children, cont->host);
    }
    if(cont->first == 0) //empty conainer
    {
	cont->first = el;
	cont->last = el;
	cont->focused = 0;
	el->next = 0;
	el->prev = 0;
	return;
    }
    MElement *m = cont->last;
    m->next = el;
    el->prev = m;
    el->next = 0;
    cont->last = el;
}

void makise_g_cont_rem(MElement *el)
{
    if(el == 0)
	return;
    
    if(el->parent == 0)
    {
	return;
    }
    MContainer *c = el->parent;

    if(c->focused == el) //if element was focused
	c->focused = 0; //reset focus
    
    if(el->prev != 0 && el->next != 0) //if element is not last or first
    {
	el->parent = 0;
	el->prev->next = el->next;
	el->next->prev = el->prev;
	el->next = el->prev = 0;
    }
    else if (el->prev == 0 && el->next != 0) //first element
    {
	el->parent = 0;
	c->first = el->next;
	el->next->prev = 0;
	el->next = 0;
    }
    else if (el->prev != 0 && el->next == 0) //last element
    {
	el->parent = 0;
	el->prev->next = 0;
	c->last = el->prev;
	el->prev = 0;
    }
    else //only member of container
    {
	el->parent = 0;
	c->last = c->first = 0;
	el->next = el->prev = 0;
    }
    el->host = 0;
    if(el->is_parent) {
	_makise_g_cont_update_host(el->children, 0);
    }

}
void makise_g_cont_clear(MContainer *c)
{
    MElement *e = c->first;
    while (e != 0) {
	makise_g_cont_rem(e);
	e = c->first;
    }
}
int32_t makise_g_cont_insert(MContainer * cont, MElement *el, uint32_t index)
{
    if(cont == 0 || el == 0)
	return -1;

    makise_g_cont_rem(el); //remove element from it's previous parent
    
    uint32_t i = 0;

    if(index == 0 || cont->first == 0) //required index is 0 or if there is no elements in container
    {
	el->parent = cont;
	el->prev = 0;
	if(cont->first == 0)
	{
	    cont->last = el;
	}
	//set queue
	el->next = cont->first;
	cont->first->prev = el;
	cont->first = el;
	//update host
	el->host = cont->host;
	if(el->is_parent) {
	    _makise_g_cont_update_host(el->children, cont->host);
	}

	return 0;
    }
    
    MElement *e = cont->first;
    while(e != 0)
    {
	i++;
	if(i == index) //if it is requested position
	{
	    el->parent = cont;
	    el->next = e->next;
	    el->prev = e;
	    //e->prev = el;
	    if(e->next == 0) //if element was last
	    {
		cont->last = el;
	    }
	    else
	    {
		e->next->prev = el;
		//e->next = el;
	    }
	    e->next = el;
	    //update host
	    el->host = cont->host;
	    if(el->is_parent) {
		_makise_g_cont_update_host(el->children,
					   cont->host);
	    }
	    
	    return i;
	}
	if(e->next == 0) //if element is last
	{
	    el->parent = cont;
	    cont->last = el;
	    e->next = el;
	    el->next = 0;
	    el->prev = e;
	    //update host
	    el->host = cont->host;
	    if(el->is_parent) {
		_makise_g_cont_update_host(el->children, cont->host);
	    }

	    return i;
	}
	e = e->next;
    }
    return -1;
}
void makise_g_cont_replace(MElement *e1, MElement *e2)
{
    if(e1 == 0 || e2 == 0 || e1 == e2)
	return;
    
    if(e1->parent == 0 || e2->parent == 0)
    {
	return;
    }

    MContainer *p1 = e1->parent,
	*p2 = e2->parent;
    
    uint32_t i1 = makise_g_cont_index(e1),
	i2 = makise_g_cont_index(e2);
    makise_g_cont_insert(p1, e2, i1);
    makise_g_cont_insert(p2, e1, i2);
}
int32_t makise_g_cont_contains(MContainer * cont, MElement *el)
{
    if(cont == 0 || el == 0)
	return -1;

    if(el->parent == 0 || //if element has no parent
       el->parent != cont) //if element's parent isn't requested container
    {
	return -1;
    }
    
    return makise_g_cont_index(el);
}
int32_t makise_g_cont_index(MElement *el)
{
    if(el == 0)
	return -1;
    
    if(el->parent == 0)
    {
	return -1;
    }
    if(el->parent->first == 0)
    {
	return -1;
    }

    uint32_t i = 0;
    MElement *e = el->parent->first;
    
    while (e != 0) {
	if(e == el)
	{
	    return i;
	}
	e = e->next;
	
	i++;
    }
    return -1;
}


uint8_t makise_g_cont_call_common_predraw(MElement *b)
{
    if(b == 0)
	return M_ZERO_POINTER;
    
    int32_t px = 0, py = 0,
	pw = b->position.width, ph = b->position.height;

    if(b->parent != 0 && b->parent->position != 0)
    {
	px = b->parent->position->real_x;
	py = b->parent->position->real_y;
	pw = b->parent->position->width;
	ph = b->parent->position->height;
    }
    
    switch (b->position.horisontal) {
    case MPositionStretch_Left:
	b->position.real_x = b->position.left + px;
    	break;
    case MPositionStretch_Right:
	b->position.real_x = (px + pw) - b->position.right - b->position.width;
    	break;
    case MPositionStretch_LeftRight:
	b->position.real_x = px + b->position.left;
	if(b->position.left + b->position.right <= pw)
	    b->position.width = pw - b->position.left - b->position.right;
	else
	    b->position.width = 0;
    	break;
    }
    switch (b->position.vertical) {
    case MPositionStretch_Up:
	b->position.real_y = b->position.up + py;
    	break;
    case MPositionStretch_Down:
	b->position.real_y = (py + ph) - b->position.down - b->position.height;
    	break;
    case MPositionStretch_UpDown:
	b->position.real_y = py + b->position.up;
	if(ph >= b->position.up + b->position.down)
	    b->position.height = ph - b->position.up - b->position.down;
	else
	    b->position.height = 0;
    	break;
    }

    return M_OK;
}
uint8_t makise_g_cont_call   (MContainer *cont, MakiseGUI *gui, uint8_t type)
{
    if(cont == 0)
	return M_ZERO_POINTER;

    if(cont->first == 0)
    {
	return M_ZERO_POINTER;
    }

    if(cont->el != 0 && cont->el->parent == 0) //container itself if it is root
    {
	if(type == M_G_CALL_PREDRAW)
	    makise_g_cont_call_common_predraw(cont->el);
    }
    
    MElement *e = cont->first;

    while(e != 0)
    {
	if(type == M_G_CALL_PREDRAW)
	    makise_g_cont_call_common_predraw(e);
	m_element_call(e, gui, type);
	e = e->next;
    }
    return M_OK;
}

MInputResultEnum makise_g_cont_input  (MContainer *cont, MInputData data)
{
    if(cont == 0)
	return M_INPUT_ERROR;
#if MAKISE_GUI_INPUT_POINTER_ENABLE == 1
    
#endif
    
    if(cont->focused != 0)
    {
	MElement *el = cont->focused;
	//MAKISE_MUTEX_REQUEST(&cont->focused->mutex_cont);
	MInputResultEnum r =
	    m_element_input(el, data);
	//MAKISE_MUTEX_RELEASE(&cont->focused->mutex_cont);
	return r;
    }

    return M_INPUT_NOT_HANDLED;
}

MFocusEnum _makise_g_cont_focus_nextprev(MContainer *cont,
					 uint8_t next,
					 uint8_t first);
MFocusEnum _makise_g_cont_focus_ord(MElement *e,
				    uint8_t next,
				    uint8_t first)
{
    uint8_t f = 1; //if current element is parent - we need to try switch it
    MElement *en;
    while (e != 0) {
	en = next ? e->next : e->prev;
	
	if(e->enabled &&
	   e->focus_prior != MFocusPrior_NotFocusble)
	{
	    //element is enabled && it requires focus
	    //if element is parent
	    uint8_t is_parent = e->is_parent;
	    MContainer *children = e->children;
	    if(is_parent && children != 0 &&
	       //children->isolated != MContainer_Isolated && //if isolated - we don't focus
	       _makise_g_cont_focus_nextprev(children, next, first)
	       == M_G_FOCUS_OK)
	    {
		return M_G_FOCUS_OK;
	    }
	    else if(!f || first)
	    {
		//if element is not parent
		if(makise_g_focus(e,
				  next ?
				  M_G_FOCUS_GET_NEXT :
				  M_G_FOCUS_GET_PREV) ==
		   M_G_FOCUS_OK)
		{
		    return M_G_FOCUS_OK;
		}
	    }
	    f = 0; //first element was tested
	}
	
	e = en;
    }
    return M_G_FOCUS_NOT_NEEDED;
}

/**
 * Changes selection to previous or next element in container
 *
 * @param cont container
 * @param next if == 0 then previous; if == 1 then next;
 * @param first if == 0 then will be normal behavior; if == 1 then current element will be passed;
 * @return Focus result MFocusEnum
 */
MFocusEnum _makise_g_cont_focus_nextprev(MContainer *cont,
					 uint8_t next, uint8_t first_t)
{
    if(cont == 0)
	return M_G_FOCUS_ERROR;

    if(cont->first == 0)
	return M_G_FOCUS_NOT_NEEDED;

    
    MElement *e = cont->first;
    uint8_t first = 1;
    if(!next)
	e = cont->last;
    if(cont->focused != 0)
    {
	if(first_t)
	{
	    if(next && cont->focused == cont->last)
	    {
		e = cont->first;
		first = 1;
	    }
	    else if(!next && cont->focused == cont->first)
	    {
		e = cont->last;
		first = 1;
	    }
	}
	else
	{
	    e = cont->focused;
	    first = 0;
	}
    }

    uint8_t cont_isolated = cont->isolated == MContainer_Isolated;
    //try to focus next element
    if(_makise_g_cont_focus_ord(e, next, first) == M_G_FOCUS_OK)
    {
	return M_G_FOCUS_OK;
    }

    //if no more elements can switch focus
    if(cont->el == 0 || cont_isolated)
    {
	//if we are root
	e = next ? cont->first : cont->last;

	MFocusEnum r = _makise_g_cont_focus_ord(e, next, 1);
	//try again
	return cont_isolated ? M_G_FOCUS_OK :
	    r;
    }
    return cont_isolated ? M_G_FOCUS_OK :
	M_G_FOCUS_NOT_NEEDED;
}
MFocusEnum makise_g_cont_focus_next(MContainer *cont)
{
    return _makise_g_cont_focus_nextprev(cont, 1, 0);
}
MFocusEnum makise_g_cont_focus_prev(MContainer *cont)
{
    return _makise_g_cont_focus_nextprev(cont, 0, 0);
}

void makise_g_cont_focus_leave(MContainer *cont)
{
    if(cont == 0)
	return;
    MElement *focused = cont->focused;
    if(cont->focused == 0)
    {
	return;
    }

    uint8_t is_parent = focused->is_parent;
    MContainer *children = focused->children;
    MFocusEnum (*focus)(MElement* el, MFocusEnum act) = focused->focus;

    //send leave event
    if(focus != 0)
	focus(focused, M_G_FOCUS_LEAVE);

    //drop focus in children
    if(is_parent && children != 0)
    {
	makise_g_cont_focus_leave(children);
    }

    //drop focused child
    cont->focused = 0;
}

MElement* makise_g_cont_element_on_point(MContainer *cont, int32_t  x, int32_t y)
{
    if(cont == 0)
	return 0;
    if(cont->first == 0)
    {
	return 0;
    }
    

    MElement *e = cont->last; //last - means upper in draw queue

    while(e != 0)
    {
	if( x >= e->position.real_x &&
	    x < e->position.real_x + (int32_t)e->position.width &&
	    y >= e->position.real_y &&
	    y < e->position.real_y + (int32_t)e->position.height   )
	{
	    //if point is in element's area
	    if(e->is_parent == 1)
	    {
		//if parent
		if(e->children != 0)
		{
		    MElement *el
			= makise_g_cont_element_on_point(
			    e->children, x, y);
		    return el;
		}
	    }
	    return e; //element found
	}
	e = e->prev;
    }
    return 0;
}

void makise_g_cont_lock(MContainer *cont)
{
    if(cont == 0 || cont->host == 0)
	return;
    MAKISE_MUTEX_REQUEST(cont->host->mutex);
}
void makise_g_cont_unlock(MContainer *cont)
{
    if(cont == 0 || cont->host == 0)
	return;
    MAKISE_MUTEX_RELEASE(cont->host->mutex);

}
