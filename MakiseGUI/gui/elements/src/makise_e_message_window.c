#include "makise_e_message_window.h"

#if ( MAKISE_E_MESSAGE_WINDOW > 0 )


static char name[] = "MessageWindow";

static MResult draw ( MElement* b, MakiseGUI *gui );

void m_create_message_window ( MMessageWindow*	 b,
                               MContainer*	 c,
                               MPosition        pos,
                               const char*      const s,
                               const MakiseStyle_SMessageWindow* style)
{
    //clear structure
    memset(b, 0, sizeof(MMessageWindow));

    b->style = style;
    b->s	 = s;

    MElement *e = &b->el;
    m_element_create(e, name, b,
                     1, 1, pos,
                     &draw,
                     0,
                     0,
                     0,
                     0,
                     0, 0);

    makise_g_cont_add(c, e);
    MAKISE_DEBUG_OUTPUT("MessageWindow %d created\n", e->id);
}

static MResult draw ( MElement* b, MakiseGUI *gui ) {
    MMessageWindow *e = ( MMessageWindow* )b->data;

    // Draw edging.
    makise_d_rect_filled( gui->buffer,
                          b->position.real_x,	b->position.real_y,
                          b->position.width,
                          b->position.height,
                          e->style->border_color,
                          e->style->bg_color );

    uint32_t height_text;
    height_text = makise_d_string_height_get( e->s, MDTextAll,
                                              b->position.width,
                                              e->style->font,
                                              e->style->font_line_spacing );

    int32_t height_offset;
    height_offset = b->position.height - height_text;
    height_offset -= 2;		 // Two line for 1 pixel each.
    if ( height_offset < 0 ) return M_OK;
    height_offset /= 2;

    makise_d_string_frame( gui->buffer,
                           e->s, MDTextAll,
                           b->position.real_x + 2,
                           b->position.real_y + height_offset,
                           b->position.width - 4,
                           height_text,
                           e->style->font,
                           e->style->font_line_spacing,
                           e->style->font_col );

    return M_OK;
}

#endif
