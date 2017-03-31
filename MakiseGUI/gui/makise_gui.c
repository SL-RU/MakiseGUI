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
    
    MContainer *p;
    if((p = el->parent) == 0)
	return M_ZERO_POINTER;

    if(el->focus_prior == 0)
	return M_G_FOCUS_NOT_NEEDED;
    
    if(event & M_G_FOCUS_GET)
    {
	//if focus need be recieved
	MElement *e = el;

	MFocusEnum r = 0; //focus result of required element
	uint8_t was = 0;

	//send event to parents
	while(e != 0)
	{
	    if(e->parent != 0)
	    {
		//if parent isn't null
		if(e->parent->focused != e)
		{
		    makise_g_cont_focus_leave(e->parent);
		}
		//set new focused
		e->parent->focused = e;
		//send focus event
		if(e->focus != 0)
		{
		    if(!was)
		    {
			//we need to get result to return only from el
			r = e->focus(e, event);
			was = 1;
			if(r == M_G_FOCUS_NOT_NEEDED)
			    return M_G_FOCUS_NOT_NEEDED;
		    }
		    else e->focus(e, event);
		}
		
		//get next element
		e = e->parent->el;
	    }
	    else
	    {
		if(e->focus != 0)
		{
		    if(!was)
		    {
			r = e->focus(e, event);
			was = 1;
			if(r == M_G_FOCUS_NOT_NEEDED)
			    return M_G_FOCUS_NOT_NEEDED;
		    }
		    else e->focus(e, event);
		}
	    }
	    
	}
	return r;
    }
    else if(event == M_G_FOCUS_LEAVE)
    {
	MElement *e = el;

	MFocusEnum r = 0;
	uint8_t was = M_G_FOCUS_NOT_NEEDED;
	
	if(el->is_parent)
	    makise_g_cont_focus_leave(el->children);
	
	while(e != 0)
	{
	    if(e->parent != 0)
	    {
		if(e->parent->focused == e)
		{
		    //if focused e
		    e->parent->focused = 0;
		    if(!was)
		    {
			r = e->focus(e, event);
			was = 1;
			if(r == M_G_FOCUS_NOT_NEEDED)
			    return M_G_FOCUS_NOT_NEEDED;
		    }
		    else e->focus(e, event);
		}
		
		e = e->parent->el;
	    }
	    else
		break;
	}
	return r;
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
