#include "makise_gui.h"


uint32_t _makise_new_id = 0;
uint32_t makise_g_newid()
{
    return _makise_new_id ++;
}


uint8_t makise_g_host_call   (MHost *host, uint8_t type)
{
    return makise_g_cont_call((host->host), type);
}

MInputResultEnum makise_g_host_input  (MHost *host, MInputData d)
{
    return makise_g_cont_input((host->host), d);
}

MFocusEnum makise_g_focus  (MElement *el, MFocusEnum event)
{
    if(el == 0)
	return M_ZERO_POINTER;

    if(event == M_G_FOCUS_LEAVE)
    {
	if(el->focus != 0)
	    el->focus(el, M_G_FOCUS_LEAVE);	
    }
    
    MContainer *p;
    if((p = el->parent) == 0)
	return M_ZERO_POINTER;

    if(event & M_G_FOCUS_GET)
    {
	//focus parent
	if(p->el != 0)
	    makise_g_focus(p->el, event);
	
	if(p->focused != el) //if not already focused
	{
	    //clear last focused element
	    if(p->focused != 0)
		makise_g_focus(p->focused, M_G_FOCUS_LEAVE);
	
	    //set current focus
	    p->focused = el;
	    //exec onfocus func
	    if(el->focus != 0)
		return el->focus(el, event);	
	}
    
    }
    else if(event == M_G_FOCUS_LEAVE)
    {
	p->focused = 0;
    }
    
    return M_G_FOCUS_OK;
}

MFocusEnum makise_g_host_focus_next(MHost *host)
{
    if(host == 0 && host->host == 0)
	return M_ZERO_POINTER;

    return makise_g_cont_focus_next(host->host);
    
}
MFocusEnum makise_g_host_focus_prev(MHost *host)
{
    if(host == 0 && host->host == 0)
	return M_ZERO_POINTER;

    return makise_g_cont_focus_next(host->host);
}

