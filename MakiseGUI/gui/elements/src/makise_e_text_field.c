#include "makise_e_text_field.h"

#if ( MAKISE_E_TEXT_FIELD > 0 )

#ifdef __cplusplus
extern "C" {
#endif

static MResult draw ( MElement* b, MakiseGUI *gui );
static MInputResultEnum input  (MElement* b, MInputData data);
static MFocusEnum focus  (MElement* b, MFocusEnum act);

static char name[] = "Text field";
void m_create_text_field(MTextField* b, MContainer *c,
			 MPosition pos,
			 char* text,
			 uint32_t len,
			 uint8_t enable_scroll,
			 MakiseStyle_TextField *style)
{
    //clear structure
    memset(b, 0, sizeof(MTextField));

    MElement *e = &b->el;
    m_element_create(e, name, b,
		     1, 1, pos,
		     &draw,
		     0,
		     0,
		     &input,
		     &focus,
		     0, 0);

    b->total_lines = UINT32_MAX;
    
    b->style = style;

    b->enable_scroll = enable_scroll;
    
    makise_g_cont_add(c, e);
    
    m_text_field_set_text(b, text, len);
    
    MAKISE_DEBUG_OUTPUT("Text field %d created\n", e->id);
}

static MResult draw ( MElement* b, MakiseGUI *gui ) {
    MTextField *t = (MTextField*)b->data;
    MakiseStyleTheme *th = t->state ?
	&t->style->normal :
	&t->style->focused;
    
    _m_e_helper_draw_box(gui->buffer, &b->position, th);

    if(t->total_lines == UINT32_MAX)
	//if total line count wasn't calculated yet
	t->total_lines = makise_d_string_get_line_count(
            gui->buffer,
	    t->text,
	    t->len,
	    t->el.position.width - 4,
	    t->style->font);
    
    char *c = makise_d_string_get_line(
        gui->buffer,
	t->text,
	t->len,
	t->current_line,
	b->position.width - 4,
	t->style->font);
    
    makise_d_string_frame(gui->buffer,
			  c,
			  t->len == MDTextAll ? MDTextAll : t->len - (c - t->text),
			  b->position.real_x + 2,// + b->position.width / 2,
			  b->position.real_y,// + b->position.height / 2,
			  b->position.width - 4
			  - ((t->enable_scroll && t->style->scroll_width)
			     ? t->style->scroll_width : 0),
			  b->position.height,
//                        MDTextPlacement_LeftUp,
			  t->style->font,
			  t->style->font_line_spacing,
			  th->font_col);

    if(!t->enable_scroll)
	return M_OK;
    // Drawing scroll.
    if (t->style->scroll_width != 0 ) {
	//calculate scroll
	uint32_t h = b->position.height - 2;
	uint32_t sh = h / t->total_lines;
	uint32_t y = 0;
	if(sh < 10)
	{
	    y = t->current_line * (h + sh - 10) / t->total_lines;
	    sh = 10;
	}
	else
	    y = t->current_line * (h) / t->total_lines;
	y += b->position.real_y + 1;

    
	makise_d_rect_filled( gui->buffer,
			      (MPoint){ b->position.real_x
                                      + b->position.width
                                      - t->style->scroll_width - 1,
                                      b->position.real_y },
			      t->style->scroll_width + 1,
			      t->el.position.height,
                              1,
			      th->border_c,
			      t->style->scroll_bg_color );

	makise_d_rect_filled( gui->buffer,
			      (MPoint){b->position.real_x
                                      + b->position.width
                                      - t->style->scroll_width - 1,
                                      y },
			      t->style->scroll_width + 1,
			      sh + 1,
                              1,
			      th->border_c,
			      t->style->scroll_color );
    }

    
    return M_OK;
}

static MInputResultEnum input  (MElement* b, MInputData data)
{
    MTextField *t = (MTextField*)b->data;
    MTextField *e = t;
    if(!e->enable_scroll)
	return M_INPUT_NOT_HANDLED;

    if(data.event == M_INPUT_CLICK)
    {
	if(data.key == M_KEY_UP)
	{
	    if(e->current_line > 0)
		e->current_line --;
	    return M_INPUT_HANDLED;
	}
	else if(data.key == M_KEY_DOWN)
	{
	    if(e->current_line < e->total_lines - 1)
		e->current_line ++;
	    else
		e->current_line = e->total_lines - 1; 
	    
	    return M_INPUT_HANDLED;
	}
    }
    return M_INPUT_NOT_HANDLED;
}

static MFocusEnum focus  (MElement* b, MFocusEnum act)
{
    MTextField *t = (MTextField*)b->data;
    MTextField *e = t;
    if(!e->enable_scroll)
	return M_G_FOCUS_NOT_NEEDED;
    
    if(act & M_G_FOCUS_GET)
    {
	if(e->state != 1)
	{
	}
	t->state = 1;
	return M_G_FOCUS_OK;
    }
    if(act == M_G_FOCUS_LEAVE)
    {
	if(e->state != 0)
	{
	}
	t->state = 0;
	return M_G_FOCUS_OK;
    }

    return M_G_FOCUS_NOT_NEEDED;
}

void m_text_field_set_text ( MTextField*   b,
			     char*         text,
			     uint32_t      len )
{
    b->text = text;
    b->len = len;
    b->total_lines = UINT32_MAX;
}

    
#ifdef __cplusplus
}
#endif

#endif
