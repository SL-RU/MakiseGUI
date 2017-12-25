#include "makise_gui.h"

#if MAKISE_MUTEX
MAKISE_MUTEX_t _makise_new_id_mutex;
#endif
static uint32_t _makise_new_id = 0;

void makise_gui_init ( MHost* host )
{
    if ( host == NULL )
        return;
    MInputHostData *inp = &host->input;

    inp->cur_buf = 0;
    inp->buf_index[0] = 0;
    inp->buf_index[1] = 0;
    inp->result_handler = 0;

#if MAKISE_GUI_INPUT_POINTER_ENABLE == 1
    inp->cursor_session = 0;
    inp->last = (MInputCursorEvent){0};
#endif
    
#if MAKISE_MUTEX
    m_mutex_create(&inp->mutex);
    m_mutex_create(&host->mutex);
    m_mutex_create(&_makise_new_id_mutex);
#endif
}

uint32_t makise_g_newid()
{
    //MAKISE_MUTEX_REQUEST(_makise_new_id_mutex);
    uint32_t id = _makise_new_id ++;
    //MAKISE_MUTEX_RELEASE(_makise_new_id_mutex);

    return id;
}


uint8_t makise_g_host_call   (MHost *host, MakiseGUI *gui, MElementCall type)
{
    MAKISE_MUTEX_REQUEST(&host->mutex);
    MContainer *cont = host->host;
    MAKISE_MUTEX_RELEASE(&host->mutex);
    
    uint8_t r = makise_g_cont_call(cont, gui, type);
    return r;
}

MInputResultEnum makise_g_host_input  (MHost *host, MInputData d)
{
    MAKISE_MUTEX_REQUEST(&host->mutex);
    MContainer *cont = host->host;
    MAKISE_MUTEX_RELEASE(&host->mutex);
    
    uint8_t r = makise_g_cont_input(cont, d);
    return r;
}

MFocusEnum makise_g_focus  (MElement *el, MFocusEnum event)
{
    if(el == 0)
	return M_ZERO_POINTER;
    
    MAKISE_MUTEX_REQUEST(&el->mutex_cont);
    
    MContainer *p = el->parent;
    if(p == 0)
    {
	MAKISE_MUTEX_RELEASE(&el->mutex_cont);
	return M_ZERO_POINTER;
    }
    
    if(el->focus_prior == 0 || el->focus == 0 || el->enabled == 0)
    {
	//if element isn't focusable
	MAKISE_MUTEX_RELEASE(&el->mutex_cont);
	return M_G_FOCUS_NOT_NEEDED;
    }

    
    if(event & M_G_FOCUS_GET)
    {
	MAKISE_DEBUG_OUTPUT("Focus get %d\n", el->id);
	
	MAKISE_MUTEX_RELEASE(&el->mutex_cont);
	
	//if focus need be recieved
	MElement *e = el,
	    *ep, //element that was mutexed
	    *en;
	
	MFocusEnum r = 0; //focus result of required element
	uint8_t was = 0;
	
	//send event to parents
	while(e != 0)
	{
	    ep = e;
	    MAKISE_MUTEX_REQUEST(&ep->mutex_cont);
	    
	    if(e->enabled == 0
	       || e->focus == 0
	       || e->focus_prior == 0)
	    {
		MAKISE_MUTEX_RELEASE(&ep->mutex_cont);
		return M_G_FOCUS_NOT_NEEDED;
	    }

	    p = e->parent;  //element's parent cintainer
	    if(p != 0)
	    {
		MAKISE_MUTEX_REQUEST(&p->mutex);   
		en = p->el;
		//if parent isn't null
		if(p->focused != e)
		{
		    MAKISE_MUTEX_RELEASE(&p->mutex);
		    makise_g_cont_focus_leave(p);
		    MAKISE_MUTEX_REQUEST(&p->mutex);
		}
		
		//set new focused
		p->focused = e;
		MAKISE_MUTEX_RELEASE(&p->mutex);	    
		//send focus event
		if(e->focus != 0)
		{
		    MAKISE_MUTEX_RELEASE(&ep->mutex_cont);
		    if(!was)
		    {
			//we need to get result to return only from el
			r = m_element_focus(e, event);
			was = 1;
			if(r == M_G_FOCUS_NOT_NEEDED)
			{
			    return M_G_FOCUS_NOT_NEEDED;
			}
		    }
		    else m_element_focus(e, event);
		}
		else
		    MAKISE_MUTEX_RELEASE(&ep->mutex_cont);
	    }
	    else
	    {
		if(e->focus != 0)
		{
		    MAKISE_MUTEX_RELEASE(&ep->mutex_cont);
		    if(!was)
		    {
			r = m_element_focus(e, event);
			was = 1;
			if(r == M_G_FOCUS_NOT_NEEDED)
			{
			    return M_G_FOCUS_NOT_NEEDED;
			}
		    }
		    else m_element_focus(e, event);
		} else
		    MAKISE_MUTEX_RELEASE(&ep->mutex_cont);
		return r;
	    }
	    
	    //get next element
	    e = en;
	}
	return r;
    }
    else if(event == M_G_FOCUS_LEAVE)
    {
	MAKISE_DEBUG_OUTPUT("Focus leave %d\n", el->id);
	
	MElement *e = el,
	    *ep, //element that was mutexed
	    *en;
	
	MFocusEnum r = 0;
	uint8_t was = M_G_FOCUS_NOT_NEEDED;

	MContainer *children = el->children;
	
	
	if(el->is_parent) {
	    MAKISE_MUTEX_RELEASE(&el->mutex_cont);
	    makise_g_cont_focus_leave(children);
	} else
	    MAKISE_MUTEX_RELEASE(&el->mutex_cont);

	
	while(e != 0)
	{
	    ep = e;
	    MAKISE_MUTEX_REQUEST(&ep->mutex_cont);
	    p = e->parent; //element's parent cintainer
	    if(p != 0)
	    {
		MAKISE_MUTEX_REQUEST(&p->mutex);	    
		en = p->el;
		if(p->focused == e)
		{
		    //if focused e
		    p->focused = 0;
		    MAKISE_MUTEX_RELEASE(&p->mutex);
		    MAKISE_MUTEX_RELEASE(&ep->mutex_cont);   

		    if(!was)
		    {
			r = m_element_focus(e, event);
			was = 1;
			if(r == M_G_FOCUS_NOT_NEEDED)
			{
			    return M_G_FOCUS_NOT_NEEDED;
			}
		    }
		    else m_element_focus(e, event);
		} else {
		    MAKISE_MUTEX_RELEASE(&p->mutex);
		    MAKISE_MUTEX_RELEASE(&ep->mutex_cont);   
		}
		e = en;
	    } else {
		MAKISE_MUTEX_RELEASE(&ep->mutex_cont);   
		break;
	    }
	}
	return r;
    }
    
    MAKISE_MUTEX_RELEASE(&el->mutex_cont);   
    return M_G_FOCUS_NOT_NEEDED;
}

MFocusEnum makise_g_host_focus_next(MHost *host)
{
    if(host == 0)
	return M_ZERO_POINTER;
    MAKISE_MUTEX_REQUEST(&host->mutex);
    MContainer *cont = host->host;
    MAKISE_MUTEX_RELEASE(&host->mutex);
    if(cont == 0)
	return M_ZERO_POINTER;
    
    MFocusEnum r = makise_g_cont_focus_next(cont);
    return r;
}
MFocusEnum makise_g_host_focus_prev(MHost *host)
{
    if(host == 0)
	return M_ZERO_POINTER;
    MAKISE_MUTEX_REQUEST(&host->mutex);
    MContainer *cont = host->host;
    MAKISE_MUTEX_RELEASE(&host->mutex);
    if(cont == 0)
	return M_ZERO_POINTER;
    
    MFocusEnum r = makise_g_cont_focus_prev(cont);
    return r;
}
void _makise_g_print_tree(MContainer *c, int l)
{
    if(c == 0)
	return;
    MAKISE_MUTEX_REQUEST(&c->mutex);
    MElement *e = c->first, *ep;
    char t[100] = {0};
    t[0] = ' ';
    if(l > 1)
    {
	for (int i = 0; i < l - 1; i++) {
	    t[i*2 + 1] = '|';
	    t[i*2 + 2] = ' ';
	}
    }
    if(l > 0)
    {
	t[l*2 - 1] = '|';
	t[l*2    ] = '-';
    }
    if(l == 0)
	t[0] = '=';
    while (e != 0)
    {
	MAKISE_MUTEX_REQUEST(&e->mutex_cont);
	MAKISE_DEBUG_OUTPUT("%s %s\tid=%d\n", t, e->name, e->id);
	if(e->is_parent && e->children != 0)
	    _makise_g_print_tree(e->children, l+1);
	ep = e;
	e = e->next;
	MAKISE_MUTEX_RELEASE(&ep->mutex_cont);
    }
    MAKISE_MUTEX_RELEASE(&c->mutex);
}
void makise_g_print_tree(MHost *host)
{
    if(host == 0)
	return;
    MAKISE_MUTEX_REQUEST(&host->mutex);
    MContainer *c = host->host;    
    _makise_g_print_tree(c, 0);
    MAKISE_MUTEX_RELEASE(&host->mutex);
}
