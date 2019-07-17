#include "makise_gui.h"

#if MAKISE_MUTEX
MAKISE_MUTEX_t _makise_new_id_mutex;
#endif
static uint32_t _makise_new_id = 0;

void makise_gui_init ( MHost* host )
{
    if ( host == NULL )
        return;

    //clear structure
    memset(host, 0, sizeof(MHost));
    
    // init gui host
    makise_g_cont_init(&host->host);
    host->host.host = host;

    //init input manager
    MInputHostData *inp = &host->input;
    inp->cur_buf = 0;
    inp->buf_index[0] = 0;
    inp->buf_index[1] = 0;
    inp->result_handler = 0;

    //init cursor input if enabled
#if MAKISE_GUI_INPUT_POINTER_ENABLE == 1
    inp->cursor_session = 0;
    inp->last = (MInputCursorEvent){0};
#endif

    //init mutexes
#if MAKISE_MUTEX
    inp->mutex = m_mutex_create();
    host->mutex = m_mutex_create();
    //m_mutex_create(&_makise_new_id_mutex);
#endif
}

void makise_gui_autoinit ( MHost *host,
                           MakiseGUI *gui,
                           MakiseDriver *driver,
                           void* (*get_buffer)(uint32_t size),
                           MInputData(*inp_handler)(MInputData d,
                                                    MInputResultEnum res),
                           void (*draw)(MakiseGUI* gui),
                           void (*predraw)(MakiseGUI* gui),
                           void (*update)(MakiseGUI* gui),
                           MResult  ( *drawer )(
                               const MakiseBuffer*,
                               const MDPrimitive*),
                           MakiseTextDrawer *text_drawer)
{
    makise_gui_init(host); //init gui host
    
    host->input.result_handler = inp_handler; //set input handler
    //set host container position and size
    host->host_size = mp_rel(0, 0,
			     driver->lcd_width,
			     driver->lcd_height);
    host->host_size.real_x = 0;
    host->host_size.real_y = 0;
    host->host.position = &host->host_size;

    uint32_t sz = makise_init(gui, driver, &gui->_buffer);
    gui->buffer->buffer = get_buffer(sz);

    gui->predraw = predraw;
    gui->draw = draw;
    gui->update = update;
    
#ifdef MAKISE_PRIMITIVES_DRAWER_DEFAULT
    if(drawer == 0)
        gui->buffer->drawer = &makise_primitives_default_drawer;
    else
        gui->buffer->drawer = drawer;
#else
    gui->buffer->drawer = drawer;
#endif
    gui->buffer->text_drawer = text_drawer;
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
    MAKISE_MUTEX_REQUEST(host->mutex);
    MContainer *cont = &host->host;
    MAKISE_MUTEX_RELEASE(host->mutex);
    
    uint8_t r = makise_g_cont_call(cont, gui, type);
    return r;
}

MInputResultEnum makise_g_host_input  (MHost *host, MInputData d)
{
    MAKISE_MUTEX_REQUEST(host->mutex);
    MContainer *cont = &host->host;
    MAKISE_MUTEX_RELEASE(host->mutex);
    
    uint8_t r = makise_g_cont_input(cont, d);
    return r;
}

MFocusEnum makise_g_focus  (MElement *el, MFocusEnum event)
{
    if(el == 0)
	return M_G_FOCUS_ERROR;
    
    MContainer *p = el->parent;
    if(p == 0)
	return M_G_FOCUS_ERROR;
    
    if(el->focus_prior == MFocusPrior_NotFocusble     // not focusable
       || (el->focus_prior == MFocusPrior_FocusbleIsolated  // is focusable but isolated
	   && event & MFocusEnum_NEXTPREV_MASK)             // and event is _NEXT or _PREV
       || el->focus == 0 || el->enabled == 0)
	// then element isn't focusable
	return M_G_FOCUS_NOT_NEEDED;

    //focus last focused child if it exist
    if(event == M_G_FOCUS_GET &&
       el->is_parent && el->children != 0 &&
       el->children->focused != 0) {
	MFocusEnum r = makise_g_focus(el->children->focused, M_G_FOCUS_GET);
	if(r == M_G_FOCUS_OK)
	    return M_G_FOCUS_OK;
    }
    
    if(event & M_G_FOCUS_GET)
    {
	MAKISE_DEBUG_OUTPUT("Focus get %d\n", el->id);
	
	//if focus need be recieved
	MElement *e = el, *en;
	
	MFocusEnum r = 0; //focus result of required element
	uint8_t was = 0;
	
	//send event to parents
	while(e != 0)
	{
	    if(e->enabled == 0
	       || e->focus == 0
//	       || (el->focus_prior == MFocusPrior_FocusbleIsolated
//		   && event & MFocusEnum_NEXTPREV_MASK)
		|| el->focus_prior == MFocusPrior_NotFocusble)
		return M_G_FOCUS_NOT_NEEDED;
	    
	    p = e->parent;  //element's parent cintainer
	    if(p != 0)
	    {
		en = p->el;
		//if parent isn't null
		if(p->focused != e)
		    makise_g_cont_focus_leave(p);
		
		//set new focused
		p->focused = e;
		//send focus event
		if(e->focus != 0)
		{
		    if(!was)
		    {
			//we need to get result to return only from el
			r = m_element_focus(e, event);
			was = 1;
			if(r == M_G_FOCUS_NOT_NEEDED)
			    return M_G_FOCUS_NOT_NEEDED;
		    }
		    else m_element_focus(e, event);
		}
	    }
	    else
	    {
		if(e->focus != 0)
		{
		    if(!was)
		    {
			r = m_element_focus(e, event);
			//was = 1;
			if(r == M_G_FOCUS_NOT_NEEDED)
			{
			    return M_G_FOCUS_NOT_NEEDED;
			}
		    }
		    else m_element_focus(e, event);
		}
		return r;
	    }
	    e = en;
	}
	return r;
    }
    else if(event == M_G_FOCUS_LEAVE)
    {
	MAKISE_DEBUG_OUTPUT("Focus leave %d\n", el->id);
	
	MElement *e = el, *en;	
	MFocusEnum r = 0;
	uint8_t was = M_G_FOCUS_NOT_NEEDED;

	MContainer *children = el->children;
	

	
	while(e != 0)
	{
	    p = e->parent; //element's parent cintainer
	    if(p != 0)
	    {
		en = p->el;
		if(p->focused == e)
		{
		    //if focused e
		    p->focused = 0;

		    if(!was)
		    {
			r = m_element_focus(e, event);
			was = 1;
			if(r == M_G_FOCUS_NOT_NEEDED)
			{
			    if(el->is_parent)
				makise_g_cont_focus_leave(children);
			    return M_G_FOCUS_NOT_NEEDED;
			}
		    }
		    else m_element_focus(e, event);
		}
		e = en;
	    } else break;
	}
	if(el->is_parent)
	    makise_g_cont_focus_leave(children);
	
	return r;
    }
    
    return M_G_FOCUS_NOT_NEEDED;
}

MFocusEnum makise_g_host_focus_next(MHost *host)
{
    if(host == 0)
	return M_G_FOCUS_ERROR;
    MContainer *cont = &host->host;
    if(cont == 0)
	return M_G_FOCUS_ERROR;
    
    MFocusEnum r = makise_g_cont_focus_next(cont);
    return r;
}
MFocusEnum makise_g_host_focus_prev(MHost *host)
{
    if(host == 0)
	return M_G_FOCUS_ERROR;
    MContainer *cont = &host->host;
    if(cont == 0)
	return M_G_FOCUS_ERROR;
    
    MFocusEnum r = makise_g_cont_focus_prev(cont);
    return r;
}
void _makise_g_print_tree(MContainer *c, int l)
{
    if(c == 0)
	return;

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
    if(l == 0)
	t[0] = '=';
    while (e != 0)
    {
	if(l > 0)
	{
	    t[l*2 - 1] = '|';

	    if(e == c->focused)
		t[l*2    ] = '*';
	    else if(c->focused == 0)
		t[l*2    ] = '-';
	    else 
		t[l*2    ] = '_';
	}
	MAKISE_DEBUG_OUTPUT("%s %s\tid=%d\n", t, e->name, e->id);
	if(e->is_parent && e->children != 0)
	    _makise_g_print_tree(e->children, l+1);
	e = e->next;
    }

}
void makise_g_print_tree(MHost *host)
{
    if(host == 0)
	return;
    MAKISE_MUTEX_REQUEST(host->mutex);
    MAKISE_DEBUG_OUTPUT("tree:\n");
    MContainer *c = &host->host;    
    _makise_g_print_tree(c, 0);
    MAKISE_MUTEX_RELEASE(host->mutex);
}
