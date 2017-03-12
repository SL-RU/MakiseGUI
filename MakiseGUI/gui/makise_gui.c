#include "makise_gui.h"


void makise_gui_init(MHost *host)
{
    if(host == 0)
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

    
}


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

    if(el->focus_prior == 0)
	return M_G_FOCUS_NOT_NEEDED;
    
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
	else
	    return M_G_FOCUS_OK; //if element already focused
    
    }
    else if(event == M_G_FOCUS_LEAVE)
    {
	p->focused = 0;
	return M_G_FOCUS_OK;
    }
    
    return M_G_FOCUS_NOT_NEEDED;
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

    return makise_g_cont_focus_prev(host->host);
}
void _makise_g_print_tree(MContainer *c, int l)
{
    MElement *e = c->first;
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
	printf("%s %s\tid=%d\n", t, e->name, e->id);
	if(e->is_parent && e->children != 0)
	    _makise_g_print_tree(e->children, l+1);
	e = e->next;
    }
}
void makise_g_print_tree(MHost *host)
{
    MContainer *c = host->host;
    _makise_g_print_tree(c, 0);
}
