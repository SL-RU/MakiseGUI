#include "makise_e_tabs.h"

#if ( MAKISE_E_TABS > 0 )

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    MakiseGUI *gui;
    MElement el;
    
    MTabs *tabs;
    
    MakiseStyle* style;
} MTabsHeader;

static uint8_t header_draw   (MElement* b);
static MInputResultEnum header_input  (MElement* b, MInputData data);
static MFocusEnum header_focus  (MElement* b, MFocusEnum act);

static uint8_t host_draw   (MElement* b);
static uint8_t host_predraw(MElement* b);
static MInputResultEnum host_input  (MElement* b, MInputData data);
static MFocusEnum host_focus  (MElement* b, MFocusEnum act);

static MResult draw (MElement* b, MakiseGUI *gui);
static uint8_t predraw(MElement* b);
static MInputResultEnum input  (MElement* b, MInputData data);
static MFocusEnum focus  (MElement* b, MFocusEnum act);

static void init_tabs(MTabs* b);

static char host_name[] = "Tabs host";
static char header_name[] = "Tabs header";
static char name[] = "Tab";
void m_create_tabs(MTabs* b, MContainer *c,
		   MPosition pos,
		   MTabs_Tab *tabs,
		   uint32_t len,
		   MTabs_Type type,
		   uint32_t size,
		   MakiseStyle *style)
{
    //clear structure
    memset(b, 0, sizeof(MTabs));

    MElement *e = &b->el;
    MElement *et = &b->tabs_el;
    MElement *eh = &b->header_el;

    MContainer *host = &b->host;
    makise_g_cont_init(host);
    
    //tab themselve
    m_element_create(et, name, b,
		     1, 1, pos,
		     &draw,
		     &predraw,
		     0,
		     &input,
		     &focus,
		     1, 0);
    //main element - host for everything else
    m_element_create(e, host_name, b,
		     1, 1, pos,
		     &host_draw,
		     &host_predraw,
		     0,
		     &host_input,
		     &host_focus,
		     1, host);
    //header
    m_element_create(eh, header_name, b,
		     1, 1, pos,
		     &header_draw,
		     0,
		     0,
		     &header_input,
		     &header_focus,
		     0, 0);

    host->el = e;
    host->position = &e->position;

    
    //add to containers
    makise_g_cont_add(c, e);
    makise_g_cont_add(host, et);
    makise_g_cont_add(host, eh);

    
    b->style = style;

    b->tabs = tabs;
    b->len = len;
    b->type = type;
    b->size = size;
    b->selected = 0;

    init_tabs(b);

    et->children = &b->tabs[b->selected].cont;

    if(type == MTabs_Type_Up)
    {
	et->position = mp_sall(0, 0, size, 0);
	eh->position = mp_shor(0, 0, 0, size);

    } else if(type == MTabs_Type_Left)
    {
	et->position = mp_sall(size, 0, 0, 0);
	eh->position = mp_sver(0, size, 0, 0);
    } else if(type == MTabs_Type_None)
    {
	et->position = mp_sall(0, 0, 0, 0);
	eh->position = mp_rel(0, 0, 0, 0);
    }
    
    MAKISE_DEBUG_OUTPUT("Tabs %d created\n", e->id);

}


static MResult draw (MElement* b, MakiseGUI *gui) {
    MakiseStyleTheme *th = 0;

    if(((MTabs*)b->data)->state == 0)
	th = &((MTabs*)b->data)->style->normal;
    else if(((MTabs*)b->data)->state == 1)
	th = &((MTabs*)b->data)->style->focused;
    
    MTabs *t = ((MTabs*)b->data);
    
    _m_e_helper_draw_box( gui->buffer, &b->position, th );

    if(t->len == 0 || t->tabs == 0)
	return M_OK;
	
    if(t->selected >= t->len)
    {
	t->selected = 0;
	b->children = &t->tabs[t->selected].cont;
    }

    return makise_g_cont_call( &t->tabs[t->selected].cont, gui, M_G_CALL_DRAW );
//    return M_OK;
}
uint8_t predraw(MElement* b)
{
    MTabs *t = ((MTabs*)b->data);

    if(t->len == 0 || t->tabs == 0)
	return M_OK;
	
    if(t->selected >= t->len)
    {
	t->selected = 0;
	b->children = &t->tabs[t->selected].cont;
    }

    return makise_g_cont_call(&t->tabs[t->selected].cont, gui, M_G_CALL_PREDRAW);
}
void _m_tabs_select_tab(MTabs *t, uint32_t s)
{
    if(t->len == 0 || t->tabs == 0)
	return;

    if(t->selected >= t->len)
    {
	t->selected = 0;
	t->tabs_el.children = &t->tabs[t->selected].cont;
    }
    if(s != t->selected)
    {
	//makise_g_focus(&t->tabs_el, M_G_FOCUS_LEAVE);
    }
    if(s < t->len)
    {
	t->selected = s;
	t->tabs_el.children = &t->tabs[t->selected].cont;
    }
}
MInputResultEnum input  (MElement* b, MInputData data)
{
    MTabs *t = ((MTabs*)b->data);

    if(t->len == 0 || t->tabs == 0)
	return M_INPUT_NOT_HANDLED;
	
    if(t->selected >= t->len)
	_m_tabs_select_tab(t, 0);

//    if(t->state == 2) //for tab selection
//	return M_INPUT_NOT_HANDLED;
    if(data.key == M_KEY_TAB_NEXT ||
       data.key == M_KEY_TAB_BACK)
	return header_input(b, data);
    
    if(t->tabs[t->selected].cont.focused == 0)
	makise_g_cont_focus_next(&t->tabs[t->selected].cont);
    if(t->tabs[t->selected].cont.focused != 0)
	return makise_g_cont_input(&t->tabs[t->selected].cont, data);
    return M_INPUT_NOT_HANDLED;
}
MFocusEnum focus  (MElement* b, MFocusEnum act)
{
    MTabs *t = ((MTabs*)b->data);

    if(t->len == 0 || t->tabs == 0)
	return M_G_FOCUS_NOT_NEEDED;
	
    if(t->selected >= t->len)
    {
	t->selected = 0;
	b->children = &t->tabs[t->selected].cont;
    }
    
    switch (act) {
    case M_G_FOCUS_GET:
	t->state = 1;
	return M_G_FOCUS_OK;
	break;
    case M_G_FOCUS_GET_NEXT:
	t->state = 1;
	return M_G_FOCUS_OK;
	break;
    case M_G_FOCUS_GET_PREV:
	t->state = 1;
	return M_G_FOCUS_OK;
	break;
    case M_G_FOCUS_LEAVE:
	t->state = 0;
	return M_G_FOCUS_OK;
	break;
    default: break;
    }
    
    return M_G_FOCUS_NOT_NEEDED;
}

void init_tabs(MTabs* b)
{
    if(b->tabs == 0)
	return;
    for (uint32_t i = 0; i < b->len; i++) {
	makise_g_cont_init(&b->tabs[i].cont);
	b->tabs[i].cont.gui = b->el.gui;
	b->tabs[i].cont.el = &b->tabs_el;
	b->tabs[i].cont.position = &b->tabs_el.position;
    }
}


static MResult draw ( MElement* b, MakiseGUI *gui ) {
{
    MTabs *t = ((MTabs*)b->data);
    
    if(t->type == MTabs_Type_Up)
    {
	uint32_t w = b->position.width / t->len;
	for (uint32_t i = 0; i < t->len; i++) {
	    makise_d_rect_filled(gui->buffer,
				 b->position.real_x + w * i, b->position.real_y,
				 w, b->position.height,
				 (t->selected != i) ?
				 t->style->unactive.border_c :
				 t->style->active.border_c,
				 (t->selected != i) ?
				 t->style->unactive.bg_color :
				 t->style->active.bg_color);
	    makise_d_string_frame(gui->buffer,
				  t->tabs[i].header , MDTextAll,
				  b->position.real_x + w * i + 3,
				  b->position.real_y, w - 6,
				  b->position.height,
				  t->style->font, 0,
				  (t->selected == i) ?
				  t->style->active.font_col :
				  t->style->unactive.font_col);
	}
    } else if(t->type == MTabs_Type_Left)
    {
	uint32_t h = b->position.height / t->len;
	for (uint32_t i = 0; i < t->len; i++) {
	    makise_d_rect_filled(gui->buffer,
				 b->position.real_x, b->position.real_y + h * i,
				 b->position.width, h,
				 (t->selected != i) ?
				 t->style->unactive.border_c :
				 t->style->active.border_c,
				 (t->selected != i) ?
				 t->style->unactive.bg_color :
				 t->style->active.bg_color);
	    makise_d_string_frame(gui->buffer,
				  t->tabs[i].header , MDTextAll,
				  b->position.real_x + 3, b->position.real_y + h * i,
				  b->position.width - 6, h,
				  t->style->font, 0,
				  (t->selected == i) ?
				  t->style->active.font_col :
				  t->style->unactive.font_col);
	}
    }
    return M_OK;
}
MInputResultEnum header_input  (MElement* b, MInputData data)
{
    MTabs *t = ((MTabs*)b->data);
    if(data.key == M_KEY_TAB_NEXT)
    {
	if(t->selected < t->len - 1)
	    _m_tabs_select_tab(t, t->selected + 1);
	else
	    _m_tabs_select_tab(t, 0);
	makise_g_focus(&t->tabs_el, M_G_FOCUS_GET);
	return M_INPUT_HANDLED;
    } else if(data.key == M_KEY_TAB_BACK)
    {
	if(t->selected >= 1)
	    _m_tabs_select_tab(t, t->selected - 1);
	else
	    _m_tabs_select_tab(t, t->len - 1);
	makise_g_focus(&t->tabs_el, M_G_FOCUS_GET);
	return M_INPUT_HANDLED;
    }
#if MAKISE_GUI_INPUT_POINTER_ENABLE == 1
    if(data.key == M_KEY_CURSOR)
    {
	int32_t y, x, v;
	switch (t->type) {
	case MTabs_Type_Left:
	    y =  data.cursor.y - b->position.real_y;
	    v = y * t->len / b->position.height;
	    if(v < t->len && v >= 0)
	    {
		_m_tabs_select_tab(t, v);
		if(data.event == M_INPUT_CLICK)
		    makise_g_focus(&t->tabs_el, M_G_FOCUS_GET);
		return M_INPUT_HANDLED;
	    }
	    break;
	case MTabs_Type_Up:
	    x =  data.cursor.x - b->position.real_x;
	    v = x * t->len / b->position.width;
	    if(v < t->len && v >= 0)
	    {
		_m_tabs_select_tab(t, v);
		if(data.event == M_INPUT_CLICK)
		    makise_g_focus(&t->tabs_el, M_G_FOCUS_GET);
		return M_INPUT_HANDLED;
	    }
	    break;
	default:
	    break;
	}
    }
#endif
    return M_INPUT_NOT_HANDLED;
}
MFocusEnum header_focus  (MElement* b, MFocusEnum act)
{
//    MTabs *t = ((MTabs*)b->data);
    switch (act) {
    case M_G_FOCUS_GET:
	return M_G_FOCUS_OK;
    case M_G_FOCUS_LEAVE:
	return M_G_FOCUS_OK;
    default: return M_G_FOCUS_NOT_NEEDED;
    }
}


uint8_t host_draw   (MElement* b)
{
    MTabs *t = ((MTabs*)b->data);
    if(t->host.focused == 0)
	t->host.focused = &(t->tabs_el);
    return makise_g_cont_call(&t->host, M_G_CALL_DRAW);
}
uint8_t host_predraw(MElement* b)
{
    MTabs *t = ((MTabs*)b->data);
    if(t->host.focused == 0)
	t->host.focused = &(t->tabs_el);
    return makise_g_cont_call(&t->host, M_G_CALL_PREDRAW);
}
MInputResultEnum host_input  (MElement* b, MInputData data)
{
    MTabs *t = ((MTabs*)b->data);
    if(t->host.focused == 0)
	t->host.focused = &(t->tabs_el);
    return makise_g_cont_input(&t->host, data);
}
MFocusEnum host_focus  (MElement* b, MFocusEnum act)
{
    MTabs *t = ((MTabs*)b->data);
    switch (act) {
    case M_G_FOCUS_GET:
	t->state = 1;
//	if(t->host.focused == 0)
//	    makise_g_cont_focus_next(&t->host);
	return M_G_FOCUS_OK;
	break;
    case M_G_FOCUS_GET_NEXT:
	t->state = 1;
//	makise_g_cont_focus_next(&t->host);
	return M_G_FOCUS_OK;
	break;
    case M_G_FOCUS_GET_PREV:
	t->state = 1;
//	makise_g_cont_focus_prev(&t->host);
	return M_G_FOCUS_OK;
	break;
    case M_G_FOCUS_LEAVE:
	t->state = 0;
	//makise_g_focus(t->host.focused, M_G_FOCUS_LEAVE);
	t->host.focused = 0;
	return M_G_FOCUS_OK;
	break;
    default: break;
    }
    return M_G_FOCUS_NOT_NEEDED;
    
}

void m_tabs_select_tab(MTabs *t, uint8_t tab)
{
    _m_tabs_select_tab(t, tab);
    makise_g_focus(&t->tabs_el, M_G_FOCUS_GET);
}
void m_tabs_select_tab_next(MTabs *t)
{
    int s = t->selected;
    s += 1;
    if(s < t->len)
	_m_tabs_select_tab(t, s);
    else
    	_m_tabs_select_tab(t, 0);
    makise_g_focus(&t->tabs_el, M_G_FOCUS_GET);
}
void m_tabs_select_tab_back(MTabs *t)
{
    int s = t->selected;
    s -= 1;
    if(s >= t->len)
	s = t->len - 1;
    if(s < 0)
	s = 0;
    _m_tabs_select_tab(t, s);
    makise_g_focus(&t->tabs_el, M_G_FOCUS_GET);
}


#ifdef __cplusplus
}
#endif

#endif
