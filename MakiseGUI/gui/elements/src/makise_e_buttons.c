#include "makise_e_buttons.h"

#if ( MAKISE_E_BUTTONS > 0 )

#ifdef __cplusplus
extern "C" {
#endif

static uint8_t 				draw   ( MElement* b );
static MInputResultEnum 	input  ( MElement* b, MInputData data );
static MFocusEnum 			focus  ( MElement* b, MFocusEnum act );

char name[] = "Button";

void m_create_button( MButton*              b,
                      MContainer*           c,
                      MPosition             pos,
                      char*                 text,
                      void                  ( *click )    ( MButton* b ),
                      uint8_t               ( *onkey )    ( MButton* b, MInputData data ),
                      void                  ( *onfocus )  ( MButton* b, MFocusEnum type ),
                      MakiseStyle_Button*    style ) {
    b->text         = text;

    b->click        = click;
    b->onkey        = onkey;
    b->onfocus      = onfocus;
    
    b->style        = style;
    
    MElement *e = &b->el;
    m_element_create        ( e, ( c == 0 ) ? 0 : c->gui, name, b, 1, 1, pos, &draw, 0, 0, &input, &focus, 0, 0 );
    makise_g_cont_add       ( c, e );
    
    MAKISE_DEBUG_OUTPUT("Button %d created\n", e->id);
}

static uint8_t draw ( MElement* b ) {
    MakiseTheme_Button* th = 0;
    MButton *e      = b->data;

    switch ( e->state ) {
        case 0:     th = &e->style->normal;                 break;
        case 1:     th = &e->style->focused;                break;
        default:    th = &e->style->active;     e->state--; break;
    }

    _m_e_helper_draw_box_param( b->gui->buffer, &b->position,
                                th->border_c, th->bg_color,th->double_border );

    makise_d_string( b->gui->buffer,                                e->text, MDTextAll,
                     b->position.real_x + b->position.width / 2,    b->position.real_y + b->position.height / 2,
                     MDTextPlacement_Center, e->style->font, th->font_col);

    //printf("Button %d dr\n", b->id);
    return M_OK;
}

static MInputResultEnum input  ( MElement* b, MInputData data ) {
    //printf("but %d inp %d %d\n", b->id, data.key, data.event);
    MButton *e          = b->data;
    uint8_t res = 1;
    if(e->onkey != 0)
    res = e->onkey(e, data);
    

    if ( ( data.key == M_KEY_OK
#if MAKISE_GUI_INPUT_POINTER_ENABLE == 1
           || data.key == M_KEY_CURSOR
#endif
          ) && data.event == M_INPUT_CLICK && e->click != 0 && res == 1 ) {
        e->state = 2;
        e->click(e);
        return M_INPUT_HANDLED;
    }
    
    if ( res == 2 )
        return M_INPUT_HANDLED;
    return M_INPUT_NOT_HANDLED;
}

static MFocusEnum focus ( MElement* b, MFocusEnum act ) {
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

void m_button_set_click   (MButton *b, void (*click   )(MButton* b))
{
    MAKISE_MUTEX_REQUEST(&b->el.mutex);
    b->click = click;
    MAKISE_MUTEX_RELEASE(&b->el.mutex);
}
void m_button_set_onkey(MButton *b, uint8_t (*onkey)(MButton* b, MInputData data))
{
    MAKISE_MUTEX_REQUEST(&b->el.mutex);
    b->onkey = onkey;
    MAKISE_MUTEX_RELEASE(&b->el.mutex);
}
void m_button_set_onfocus (MButton *b, void (*onfocus )(MButton* b, MFocusEnum type))
{
    MAKISE_MUTEX_REQUEST(&b->el.mutex);
    b->onfocus = onfocus;
    MAKISE_MUTEX_RELEASE(&b->el.mutex);
}
void m_button_set_text (MButton *b, char * c)
{
    MAKISE_MUTEX_REQUEST(&b->el.mutex);
    b->text = c;
    MAKISE_MUTEX_RELEASE(&b->el.mutex);
}

#ifdef __cplusplus
}
#endif

#endif
