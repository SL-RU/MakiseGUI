#include "makise_e_slider.h"

#if MAKISE_E_SLIDER > 0

static uint8_t draw   (MElement* b, MakiseGUI *gui);
static MInputResultEnum input  (MElement* b, MInputData data);
static MFocusEnum focus  (MElement* b, MFocusEnum act);

static char name[] = "Slider";
void m_create_slider(MSlider* b, MContainer *c,
		     MPosition pos,
		     MSlider_Type type,
		     const MakiseStyle *style)
{
    //clear structure
    memset(b, 0, sizeof(MSlider));
    
    MElement *e = &b->el;

    m_element_create(e, name, b,
		     1, MFocusPrior_Focusble, pos,
		     &draw,
		     0,
		     0,
		     &input,
		     &focus,
		     0, 0);

    b->type = type;    
    b->style = style;

    b->value_p = &b->value;
    
    makise_g_cont_add(c, e);

#if ( MAKISE_ENABLE_DEBUG_OUTPUT > 0 )
    MAKISE_DEBUG_OUTPUT("Slider %d created\n", e->id);
#endif
}

static uint8_t draw   (MElement* b, MakiseGUI *gui)
{
    const MakiseStyleTheme *th = 0;

    MSlider *s = ((MSlider*)b->data);

    /* if(s->value_p == 0) */
    /* 	return M_OK; */
    
    if(((MSlider*)b->data)->state == 0)
	th = &((MSlider*)b->data)->style->normal;
    else //if(((MSlider*)b->data)->state == 1)
	th = &((MSlider*)b->data)->style->focused;

    if(s->type == MSlider_Type_Read)
	th = &((MSlider*)b->data)->style->focused;
    
    _m_e_helper_draw_box(gui->buffer, &b->position, th);

    if(_MSlider_is_horizontal(b))
    {
	int32_t val = (*(s->value_p) - s->value_min) * (b->position.width - 5) /
	    (s->value_max - s->value_min);
	//borders
	makise_d_rect_filled(gui->buffer,
			     (MPoint){b->position.real_x,
                                     b->position.real_y},
			     3,
			     b->position.height,
                             th->thickness,
			     th->border_c,
			     th->border_c);
	makise_d_rect_filled(gui->buffer,
			     (MPoint){b->position.real_x + b->position.width - 3,
                                     b->position.real_y},
			     3,
			     b->position.height,
                             th->thickness,
			     th->border_c,
			     th->border_c);
	//filled part
	makise_d_rect_filled(gui->buffer,
			     (MPoint){b->position.real_x + 2,
                                     b->position.real_y + b->position.height/6},
			     val,
			     b->position.height*4/6,
                             th->thickness,
			     th->font_col,
			     th->font_col);
	//empty part
	makise_d_rect(gui->buffer,
		      (MPoint){b->position.real_x + val + 3,
                              b->position.real_y + b->position.height/6},
		      b->position.width - val - 5,
		      b->position.height*4/6,
                      th->thickness,
		      th->font_col);
	if(s->type == MSlider_Type_ReadWrite)
	    //line
	    makise_d_line(gui->buffer,
			  (MPoint){b->position.real_x + val + 2,
                                  b->position.real_y},
			  (MPoint){b->position.real_x + val + 2,
                                  b->position.real_y + b->position.height - 1},
                          th->thickness,
			  th->font_col);

    }
    else
    {
	int32_t val = (*s->value_p - s->value_min) * (b->position.height - 5) /
	    (s->value_max - s->value_min);
	val = b->position.height - val - 5;
	//borders
	makise_d_rect_filled(gui->buffer,
			     (MPoint){b->position.real_x,
                                     b->position.real_y},
			     b->position.width,
			     3,
                             th->thickness,
			     th->font_col,
			     th->font_col);
	makise_d_rect_filled(gui->buffer,
			     (MPoint){b->position.real_x,
                                     b->position.real_y + b->position.height - 3},
			     b->position.width,
			     3,
                             th->thickness,
			     th->font_col,
			     th->font_col);
	//filled part
	makise_d_rect(gui->buffer,
		      (MPoint){b->position.real_x + b->position.width/6,
                              b->position.real_y + 2},
		      b->position.width*4/6,
		      val,
                      th->thickness,
		      th->font_col);
	//empty part
	makise_d_rect_filled(gui->buffer,
			     (MPoint){b->position.real_x + b->position.width/6,
                                     b->position.real_y + val + 3},
			     b->position.width*4/6,
			     b->position.height - val - 5,
                             th->thickness,
			     th->font_col,
			     th->font_col);
	
	if(s->type == MSlider_Type_ReadWrite)
	    //line
	    makise_d_line(gui->buffer,
			  (MPoint){b->position.real_x,
                                  b->position.real_y + val + 2},
			  (MPoint){b->position.real_x + b->position.width - 1,
                                  b->position.real_y + val + 2},
                          th->thickness,
			  th->font_col);

    }
    
    //makise_d_string(gui->buffer,
//		    ((MSlider*)b->data)->text, MDTextAll,
//		    b->position.real_x + b->position.width / 2,
//		    b->position.real_y + b->position.height / 2,
//		    MDTextPlacement_Center,
//		    ((MSlider*)b->data)->style->font,
//		    th->font_col);

    //printf("Slider %d dr\n", b->id);
    return M_OK;
}

static MInputResultEnum input  (MElement* b, MInputData data)
{
    MSlider *e = ((MSlider*)b->data);
    if(e->type == MSlider_Type_Read)
	return M_INPUT_NOT_HANDLED;

    if(data.event == M_INPUT_CLICK || ((e->value_max - e->value_min) > 20 &&
				       data.time > 400))
    {
	if(data.key == (_MSlider_is_horizontal(b) ? M_KEY_RIGHT : M_KEY_UP))
	{
	    if(*e->value_p + 1 > e->value_max)
		*e->value_p = e->value_max;
	    else
		(*e->value_p) ++;
	    if(e->onchange != 0)
		e->onchange(e, *e->value_p);

	    return M_INPUT_HANDLED;
	}
	else if(data.key == (_MSlider_is_horizontal(b) ? M_KEY_LEFT : M_KEY_DOWN))
	{
	    if(*e->value_p - 1 < e->value_min)
		*e->value_p = e->value_min;
	    else
		(*e->value_p) --;
	    if(e->onchange != 0)
		e->onchange(e, *e->value_p);
	    
	    return M_INPUT_HANDLED;
	}
    }
#if MAKISE_GUI_INPUT_POINTER_ENABLE == 1
    if(data.key == M_KEY_CURSOR)
    {
	int32_t x, y, v;
	if(_MSlider_is_horizontal(b))
	{
	    x =  data.cursor.x - b->position.real_x;
	    v = x * (e->value_max - e->value_min) / b->position.width;
	    v += e->value_min;
	    if(v >= e->value_min && v <= e->value_max)
	    {
		*e->value_p = v;
		if(e->onchange != 0)
		    e->onchange(e, *e->value_p);
	    }
	    return M_INPUT_HANDLED;
	}
	else if(!_MSlider_is_horizontal(b))
	{
	    y =  data.cursor.y - b->position.real_y;
	    v = y * (e->value_max - e->value_min) / b->position.height;
	    v = (e->value_max - e->value_min) - v;
	    v += e->value_min;
	    if(v >= e->value_min && v <= e->value_max)
	    {
		*e->value_p = v;
		if(e->onchange != 0)
		    e->onchange(e, *e->value);
	    }
	    return M_INPUT_HANDLED;
	}

    }
#endif
    return M_INPUT_NOT_HANDLED;
}
static MFocusEnum focus  (MElement* b, MFocusEnum act)
{
    MSlider *e = ((MSlider*)b->data);
    if(e->type == MSlider_Type_Read)
	return M_G_FOCUS_NOT_NEEDED;
    
    if(act & M_G_FOCUS_GET)
    {
	if(e->state != 1 && e->onfocus != 0)
	{
	    e->onfocus(e, M_G_FOCUS_GET);
	}
	((MSlider*)b->data)->state = 1;
	return M_G_FOCUS_OK;
    }
    if(act == M_G_FOCUS_LEAVE)
    {
	if(e->state != 0 && e->onfocus != 0)
	{
	    e->onfocus(e, M_G_FOCUS_LEAVE);
	}
	((MSlider*)b->data)->state = 0;
	return M_G_FOCUS_OK;
    }

    return M_G_FOCUS_NOT_NEEDED;
}

void m_slider_set_range(MSlider* b, int32_t i, int32_t j)
{
    m_element_mutex_request(&b->el);

    if(j > i)
    {
	b->value_max = j;
	b->value_min = i;	
    }
    else
    {
	b->value_max = j;
	b->value_min = i;
    }
    int32_t v = *b->value_p;
    m_element_mutex_release(&b->el);

    m_slider_set_value(b, v);
}
void m_slider_set_value(MSlider* b, int32_t v)
{
    m_element_mutex_request(&b->el);
    
    if(v > b->value_max)
	*b->value_p = b->value_max;
    else if(v < b->value_min)
	*b->value_p = b->value_min;
    else
	*b->value_p = v;
    
    m_element_mutex_release(&b->el);
}
int32_t m_slider_get_value(MSlider* b)
{
    int32_t v;
    m_element_mutex_request(&b->el);
    v = *b->value_p;
    m_element_mutex_release(&b->el);
    return v;
}
void m_slider_set_onchange(MSlider* b, void (*onchange)(MSlider* b, int32_t val))
{
    m_element_mutex_request(&b->el);
    b->onchange = onchange;
    m_element_mutex_release(&b->el);
}
void m_slider_set_onfocus (MSlider* b, void (*onfocus )(MSlider* b, MFocusEnum type))
{
    m_element_mutex_request(&b->el);
    b->onfocus = onfocus;
    m_element_mutex_release(&b->el);
}

#endif
