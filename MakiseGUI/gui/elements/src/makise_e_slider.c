#include "makise_e.h"

static uint8_t draw   (MElement* b);
static MInputResultEnum input  (MElement* b, MInputData data);
static MFocusEnum focus  (MElement* b, MFocusEnum act);

static char name[] = "Slider";
void m_create_slider(MSlider* b, MContainer *c,
		     MPosition pos,
		     int32_t *value,
		     int32_t value_max,
		     int32_t value_min,
		     MSlider_Type type,
		     void    (*onchange   )(MSlider* b, int32_t val),
		     void    (*onfocus )(MSlider* b, MFocusEnum type),
		     MakiseStyle *style)
{
    MElement *e = &b->el;

    m_element_create(e, (c == 0) ? 0 : c->gui, name, b,
		     1, 1, pos,
		     &draw,
		     0,
		     0,
		     &input,
		     &focus,
		     0, 0);

    if(value_min > value_max)
    {
	b->value_max = value_min;
	b->value_min = value_max;	
    }
    else
    {
	b->value_max = value_max;
	b->value_min = value_min;	
    }
    b->value = value;
    if(*b->value > b->value_max)
	*b->value = b->value_max;
    if(*b->value < b->value_min)
	*b->value = b->value_min;

    b->type = type;
    
    b->onchange = onchange;
    b->onfocus = onfocus;
    
    b->style = style;
    
    makise_g_cont_add(c, e);
    
    printf("Slider %d created\n", e->id);
}

static uint8_t draw   (MElement* b)
{
    MakiseStyleTheme *th = 0;

    MSlider *s = ((MSlider*)b->data);
    
    if(((MSlider*)b->data)->state == 0)
	th = &((MSlider*)b->data)->style->normal;
    else if(((MSlider*)b->data)->state == 1)
	th = &((MSlider*)b->data)->style->focused;

    if(s->type == MSlider_Type_Read)
	th = &((MSlider*)b->data)->style->focused;
    
    _m_e_helper_draw_box(b->gui->buffer, &b->position, th);

    if(_MSlider_is_horizontal(b))
    {
	int32_t val = (*s->value - s->value_min) * (b->position.width - 5) /
	    (s->value_max - s->value_min);
	//borders
	makise_d_rect_filled(b->gui->buffer,
			     b->position.real_x,
			     b->position.real_y,
			     3,
			     b->position.height,
			     th->border_c,
			     th->border_c);
	makise_d_rect_filled(b->gui->buffer,
			     b->position.real_x + b->position.width - 3,
			     b->position.real_y,
			     3,
			     b->position.height,
			     th->border_c,
			     th->border_c);
	//filled part
	makise_d_rect_filled(b->gui->buffer,
			     b->position.real_x + 2,
			     b->position.real_y + b->position.height/6,
			     val,
			     b->position.height*4/6,
			     th->font_col,
			     th->font_col);
	//empty part
	makise_d_rect(b->gui->buffer,
		      b->position.real_x + val + 3,
		      b->position.real_y + b->position.height/6,
		      b->position.width - val - 5,
		      b->position.height*4/6,
		      th->font_col);
	if(s->type == MSlider_Type_ReadWrite)
	    //line
	    makise_d_line(b->gui->buffer,
			  b->position.real_x + val + 2,
			  b->position.real_y,
			  b->position.real_x + val + 2,
			  b->position.real_y + b->position.height - 1,
			  th->font_col);

    }
    else
    {
	int32_t val = (*s->value - s->value_min) * (b->position.height - 5) /
	    (s->value_max - s->value_min);
	val = b->position.height - val - 5;
	//borders
	makise_d_rect_filled(b->gui->buffer,
			     b->position.real_x,
			     b->position.real_y,
			     b->position.width,
			     3,
			     th->font_col,
			     th->font_col);
	makise_d_rect_filled(b->gui->buffer,
			     b->position.real_x,
			     b->position.real_y + b->position.height - 3,
			     b->position.width,
			     3,
			     th->font_col,
			     th->font_col);
	//filled part
	makise_d_rect(b->gui->buffer,
			     b->position.real_x + b->position.width/6,
			     b->position.real_y + 2,
			     b->position.width*4/6,
			     val,
			     th->font_col);
	//empty part
	makise_d_rect_filled(b->gui->buffer,
			     b->position.real_x + b->position.width/6,
			     b->position.real_y + val + 3,
			     b->position.width*4/6,
			     b->position.height - val - 5,
			     th->font_col,
			     th->font_col);
	
	if(s->type == MSlider_Type_ReadWrite)
	    //line
	    makise_d_line(b->gui->buffer,
			  b->position.real_x,
			  b->position.real_y + val + 2,
			  b->position.real_x + b->position.width - 1,
			  b->position.real_y + val + 2,
			  th->font_col);

    }
    
    //makise_d_string(b->gui->buffer,
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
	    if(*e->value + 1 > e->value_max)
		*e->value = e->value_max;
	    else
		(*e->value) ++;
	    if(e->onchange != 0)
		e->onchange(e, *e->value);

	    return M_INPUT_HANDLED;
	}
	else if(data.key == (_MSlider_is_horizontal(b) ? M_KEY_LEFT : M_KEY_DOWN))
	{
	    if(*e->value - 1 < e->value_min)
		*e->value = e->value_min;
	    else
		(*e->value) --;
	    if(e->onchange != 0)
		e->onchange(e, *e->value);
	    
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
		*e->value = v;
		if(e->onchange != 0)
		    e->onchange(e, *e->value);
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
		*e->value = v;
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

