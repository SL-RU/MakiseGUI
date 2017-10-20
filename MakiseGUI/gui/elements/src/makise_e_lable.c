#include "makise_e_lable.h"

#if MAKISE_E_LABLE > 0

static uint8_t draw   ( MElement* b, MakiseGUI *gui );

static char name[] = "Lable";

void m_create_lable( MLable*             b,
                     MContainer*         c,
                     MPosition           pos,
                     MakiseStyle_Lable*   style ) {
    MElement *e = &b->el;

    m_element_create(e, name, b,
		     1, 1, pos,
		     &draw,
		     0,
		     0,
		     0,
		     0,
		     0, 0);
    
    b->text = 0;
    b->style = style;

    makise_g_cont_add(c, e);
    
    MAKISE_DEBUG_OUTPUT("Lable %d created\n", e->id);
}

static uint8_t draw ( MElement* b, MakiseGUI *gui ) {
    MLable *e = b->data;
    
    _m_e_helper_draw_box_param( gui->buffer, &b->position,
                                e->style->border_c, e->style->bg_color, e->style->double_border );


    if ( e->text != 0 )
        makise_d_string(gui->buffer,
                e->text, MDTextAll,
                b->position.real_x + 2,         // + b->position.width / 2,
                b->position.real_y,             // + b->position.height / 2,
                MDTextPlacement_LeftUp,
                e->style->font,
                e->style->font_col);
    
    return M_OK;
}

void m_lable_set_text( MLable *b,
		       char   *text)
{
    MAKISE_MUTEX_REQUEST(&b->el.mutex);

    b->text = text;
    
    MAKISE_MUTEX_RELEASE(&b->el.mutex);
}

char* m_lable_get_text( MLable *b )
{
    MAKISE_MUTEX_REQUEST(&b->el.mutex);

    char *t = b->text;
    
    MAKISE_MUTEX_RELEASE(&b->el.mutex);
    
    return t;
}

#endif
