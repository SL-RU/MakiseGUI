#include "makise_e_buttons.h"

#if MAKISE_E_BUTTONS > 0

static MResult                  draw   (MElement* b, MakiseGUI *gui);
static MInputResultEnum         input  (MElement* b, MInputData data);
static MFocusEnum               focus  (MElement* b, MFocusEnum act);


char name[] = "Button";

void m_create_button( MButton*              b,
                      MContainer*           c,
                      MPosition             pos,
                      MakiseStyle_Button*    style )
{
    //clear structure
    memset(b, 0, sizeof(MButton));

    b->text         = 0;

    b->click        = 0;
    b->onkey        = 0;
    b->onfocus      = 0;
    
    b->style        = style;
    
    MElement *e = &b->el;
    m_element_create
	(e, name, b,
	 1, MFocusPrior_Focusble,
	 pos, &draw,
	 0, 0,
	 &input, &focus,
	 0, 0);
    makise_g_cont_add       ( c, e );
    
    MAKISE_DEBUG_OUTPUT("Button %d created\n", e->id);
}

static MResult draw ( MElement* b, MakiseGUI *gui )
{
    MakiseStyleTheme* th = 0;
    MButton *e      = b->data;

    switch ( e->state ) {
        case 0:     th = &e->style->normal;              break;
        case 1:     th = &e->style->focused;             break;
        default:    th = &e->style->active;  e->state--; break;
    }
    
    _m_e_helper_draw_box_param( gui->buffer, &b->position,
                                th->border_c, th->bg_color,th->thickness );

    uint32_t wt = makise_d_string_get_width(gui->buffer,
                                            e->text, MDTextAll, e->style->font),
	wb = 0, w = 0, sx = 3, bity = 0;

    if(e->bitmap != 0)
    {
	wb += (wt ? e->style->bitmap_gap : 0)
	    + e->bitmap->width ;
	bity = (b->position.height - e->bitmap->height)/2;
    }

    w = wb + wt;

    if(b->position.width >= w)
    {
	sx = (b->position.width - w) / 2;

	if(e->text != 0)
	    makise_d_string( gui->buffer, e->text, MDTextAll,
			     b->position.real_x + b->position.width / 2 + wb/2,
			     b->position.real_y + b->position.height / 2,
			     MDTextPlacement_Center, e->style->font, th->font_col);
	if(e->bitmap != 0)
	    makise_d_bitmap( gui->buffer,
			     b->position.real_x + sx,
			     b->position.real_y + bity,
			     e->bitmap, th->font_col );
    }
    else
    {
	sx = 3;

	if(e->bitmap != 0)
	    makise_d_bitmap( gui->buffer,
			     b->position.real_x + sx,
			     b->position.real_y + bity,
			     e->bitmap, th->font_col );
	
	bity = (b->position.height - e->style->font->height)/2;

	if(e->text != 0)
	    makise_d_string( gui->buffer, e->text, MDTextAll,
			     b->position.real_x + wb + sx,
			     b->position.real_y + bity,
			     MDTextPlacement_LeftUp, e->style->font, th->font_col);
    }
    
    //printf("Button %d dr\n", b->id);
    return M_OK;
}

static MInputResultEnum input  ( MElement* b, MInputData data )
{
    MButton *e          = b->data;
    uint8_t res = M_INPUT_NOT_HANDLED;
    if(e->onkey != 0)
	res = e->onkey(e, data);
    

    if ( ( data.key == M_KEY_OK
#if MAKISE_GUI_INPUT_POINTER_ENABLE == 1
           || data.key == M_KEY_CURSOR
#endif
          ) && data.event == M_INPUT_CLICK && e->click != 0 && res == 1 ) {
        e->state = e->style->active_delay;
        e->click(e);
        return M_INPUT_HANDLED;
    }
    
    if ( res == 2 )
        return M_INPUT_HANDLED;
    return M_INPUT_NOT_HANDLED;
}

static MFocusEnum focus ( MElement* b, MFocusEnum act )
{
    MButton *e =        b->data;
    if ( act & M_G_FOCUS_GET ) {
        if(e->state != 1 && e->onfocus != 0)
            e->onfocus(e, M_G_FOCUS_GET);

        e->state = 1;
        MAKISE_DEBUG_OUTPUT( "but get %d\n", b->id );
    }

    if( act == M_G_FOCUS_LEAVE ) {
        if( e->state != 0 && e->onfocus != 0 )
            e->onfocus(e, M_G_FOCUS_LEAVE);
        e->state = 0;
        MAKISE_DEBUG_OUTPUT("but leave %d\n", b->id);
    }
    //printf("but %d foc %d\n", b->id, act);
    return ( act == M_G_FOCUS_PREV || act == M_G_FOCUS_NEXT ) ? M_G_FOCUS_NOT_NEEDED : M_G_FOCUS_OK;
}

void m_button_set_click(MButton *b, void (*click)(MButton* b))
{
    M_E_MUTEX_REQUEST(b);
    b->click = click;
    M_E_MUTEX_RELEASE(b);
}
void m_button_set_onkey(MButton *b, uint8_t (*onkey)(MButton* b, MInputData data))
{
    M_E_MUTEX_REQUEST(b);
    b->onkey = onkey;
    M_E_MUTEX_RELEASE(b);
}
void m_button_set_onfocus (MButton *b, void (*onfocus )(MButton* b, MFocusEnum type))
{
    M_E_MUTEX_REQUEST(b);
    b->onfocus = onfocus;
    M_E_MUTEX_RELEASE(b);
}
void m_button_set_text (MButton *b, char * c)
{
    M_E_MUTEX_REQUEST(b);
    b->text = c;
    M_E_MUTEX_RELEASE(b);
}
void m_button_set_bitmap  (MButton *b, const MakiseBitmap *bm)
{
    M_E_MUTEX_REQUEST(b);
    M_E_MUTEX_RELEASE(b);
    b->bitmap = bm;
}

#endif
