#include "makise_gui.h"

uint8_t makise_g_host_call   (MHost *host, uint8_t type)
{
    return makise_g_cont_call((host->host), type);
}
MInputResultEnum makise_g_host_input  (MHost *host, MInputData d)
{
    return makise_g_cont_input((host->host), d);
}

MFocusEnum makise_g_focus  (MElement *el)
{
    MContainer *c;
    if(el == 0) //if zero pointer
	return M_G_FOCUS_ERROR;
    c = el->parent;
    if(c == 0) //if parent doesn't assigned
	return M_G_FOCUS_ERROR;
    if(c->first == 0) //if parent doesn't have children
	return M_G_FOCUS_ERROR;

    MElement *f = el;
    while (1)
    {
	if(f->parent == 0) //if parent doesn't assigned
	    return M_G_FOCUS_OK; //done
	c = f->parent;
	if(c->first == 0) //if parent doesn't have children
	    return M_G_FOCUS_ERROR;

	if(c->focused != el && c->focused != 0)
	{
	    c->focused->focus(c->focused, M_G_FOCUS_LEAVE);
	}
	c->focused = f;
	f->focus(f, M_G_FOCUS_GET);

	if(c->el == 0)
	    return M_G_FOCUS_OK;
	f = c->el; //lower parent's elemetn
    }
    
    
    return M_G_FOCUS_OK;
}
uint32_t _makise_new_id = 0;
uint32_t makise_g_newid()
{
    return _makise_new_id ++;
}
