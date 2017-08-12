#include "makise_e_canvas.h"

#if ( MAKISE_E_CANVAS > 0 )

#ifdef __cplusplus
extern "C" {
#endif

static uint8_t draw   (MElement* b);
static uint8_t predraw(MElement* b);
static MInputResultEnum input  (MElement* b, MInputData data);
static MFocusEnum focus  (MElement* b, MFocusEnum act);

static char name[] = "Canvas";
void m_create_canvas( MCanvas*            b,
                      MContainer*         c,
                      MPosition           pos,
                      MakiseStyle_Canvas*  style ) {
    MElement *e = &b->el;

    m_element_create(e, (c == 0) ? 0 : c->gui, name, b,
		     1, 1,
		     pos,
		     &draw,
		     &predraw,
		     0,
		     &input,
		     &focus,
		     1, &b->cont);
    
    
    b->style = style;

    makise_g_cont_init( &b->cont );
    
    b->cont.gui         = c->gui;
    b->cont.el          = e;
    b->cont.position    = &e->position;

    makise_g_cont_add(c, e);
    
    MAKISE_DEBUG_OUTPUT("Canvas %d created\n", e->id);
}


static uint8_t draw (MElement* b)
{
    MakiseStyleTheme_Canvas*     th = 0;
    MCanvas*                    c  = b->data;

    switch( c->state ) {
        case 0:     th = &c->style->normal;     break;
        default:    th = &c->style->focused;    break;
    }

    _m_e_helper_draw_box_param( b->gui->buffer, &b->position,
                                th->border_c, th->bg_color,th->double_border );

    //printf("Canvas %d dr\n", b->id);
    return makise_g_cont_call(&c->cont, M_G_CALL_DRAW);
//    return M_OK;
}

static uint8_t predraw (MElement* b) {
    MCanvas*            c  = b->data;
    return makise_g_cont_call(&c->cont, M_G_CALL_PREDRAW);
}

static MInputResultEnum input (MElement* b, MInputData data) {
    MCanvas*            c  = b->data;
    if(c->cont.focused == 0)
    makise_g_cont_focus_next(&(c->cont));
    if(c->cont.focused != 0)
    return makise_g_cont_input(&c->cont, data);
    return M_INPUT_NOT_HANDLED;
}

static MFocusEnum focus (MElement* b, MFocusEnum act) {
    MCanvas*            c  = b->data;
    switch (act) {
        case M_G_FOCUS_GET:
            c->state = 1;
            return M_G_FOCUS_OK;

        case M_G_FOCUS_GET_NEXT:
            return M_G_FOCUS_NOT_NEEDED;

        case M_G_FOCUS_GET_PREV:
            return M_G_FOCUS_NOT_NEEDED;

        case M_G_FOCUS_LEAVE:
            c->state = 0;
            c->cont.focused = 0;
            return M_G_FOCUS_OK;

        case M_G_FOCUS_NEXT:
            return M_G_FOCUS_OK;

        case M_G_FOCUS_PREV:
            return M_G_FOCUS_OK;

        default:
            return M_G_FOCUS_NOT_NEEDED;
    }   
}

#ifdef __cplusplus
}
#endif

#endif

