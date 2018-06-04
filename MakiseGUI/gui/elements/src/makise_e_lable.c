#include "makise_e_lable.h"

#if MAKISE_E_LABLE > 0

static MResult draw   ( MElement* b, MakiseGUI *gui );

static char name[] = "Lable";

void m_create_lable( MLable*             b,
                     MContainer*         c,
                     MPosition           pos,
                     MakiseStyle_Lable*   style )
{
    //clear structure
    memset(b, 0, sizeof(MLable));
    
    MElement *e = &b->el;

    m_element_create(e, name, b,
		     1, MFocusPrior_Focusble, pos,
		     &draw,
		     0,
		     0,
		     0,
		     0,
		     0, 0);
    
    b->text = 0;
    b->style = style;
    b->scroll_enable = 1; //enable scroll by default
    b->scroll_x = 0;
    b->alignment = MDTextPlacement_LeftCenter;

    makise_g_cont_add(c, e);
    
    
    MAKISE_DEBUG_OUTPUT("Lable %d created\n", e->id);
}

static MResult draw ( MElement* b, MakiseGUI *gui )
{
    MLable *e = b->data;
    
    _m_e_helper_draw_box_param( gui->buffer, &b->position,
                                e->style->border_c, e->style->bg_color, e->style->thickness );

    if(e->text_width == UINT32_MAX)
        e->text_width = makise_d_string_get_width(gui->buffer, e->text,
                                                  MDTextAll, e->style->font);
    
    if ( e->text != 0 ) {
	int32_t dx = 0, scrlx = e->scroll_x / 100;
	if(e->scroll_enable && b->position.width < e->text_width)
	{
	    if(scrlx >= (int32_t)e->text_width) {
		e->scroll_x = 0;
		dx = 0;
	    }
	    else
	    {
		if(scrlx < (int32_t)b->position.width / 2)
		    dx = 0;
		else if(scrlx >= (int32_t)(e->text_width - (b->position.width / 4)))
		    dx = -(e->text_width - (b->position.width / 4))
			+ b->position.width / 2;
		else
		    dx = -scrlx + b->position.width / 2;
	    }
	    e->scroll_x += e->style->scroll_speed;
	}
        makise_d_string(gui->buffer,
			e->text, MDTextAll,
			(int32_t)(b->position.real_x + 2 + dx),
			b->position.real_y + b->position.height / 2,
			MDTextPlacement_LeftCenter,
			e->style->font,
			e->style->font_col);
    }
    
    return M_OK;
}

void m_lable_set_text( MLable *b,
		       char   *text)
{
    M_E_MUTEX_REQUEST(b);
    
    b->text = text;
    b->text_width = UINT32_MAX;
    b->scroll_x = 0;
    
    M_E_MUTEX_RELEASE(b);
}

char* m_lable_get_text( MLable *b )
{
    M_E_MUTEX_REQUEST(b);

    char *t = b->text;
    
    M_E_MUTEX_RELEASE(b);
    return t;
}

void m_lable_enable_scroll( MLable *b, uint8_t enable )
{
    M_E_MUTEX_REQUEST(b);    
    
    b->scroll_enable = enable;
    b->scroll_x = 0;
    
    M_E_MUTEX_RELEASE(b);
}

/* void m_lable_set_alignment( MLable *b, */
/*                             MDTextPlacement alignment) */
/* { */
/*     M_E_MUTEX_REQUEST(b);     */
    
/*     b->alignment = alignment; */
    
/*     M_E_MUTEX_RELEASE(b); */
/* } */


#endif
