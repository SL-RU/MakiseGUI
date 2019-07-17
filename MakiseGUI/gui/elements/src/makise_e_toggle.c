#include "makise_e_toggle.h"

#if ( MAKISE_E_TOGGLE > 0 )

#ifdef __cplusplus
extern "C" {
#endif

static MResult draw   (MElement* b, MakiseGUI *gui);
static MInputResultEnum input  (MElement* b, MInputData data);
static MFocusEnum focus  (MElement* b, MFocusEnum act);

static char name[] = "Toggle";
void m_create_toggle(MToggle* b, MContainer *c,
		     MPosition pos,
		     char* text,
		     void (*toggled)(MToogle* b, uint8_t state),
		     MakiseStyle *style)
{
    //clear structure
    memset(b, 0, sizeof(MToggle));

    MElement *e = &b->el;

    m_element_create(e, name, b,
		     1, 1, pos,
		     &draw,
		     0,
		     0,
		     &input,
		     &focus,
		     0, 0);
    
    b->text = text;

    b->toggled = toggled;
    
    b->style = style;
    
    makise_g_cont_add(c, e);
    
    MAKISE_DEBUG_OUTPUT("Toggle %d created\n", e->id);
}

MResult draw   (MElement* b, MakiseGUI *gui)
{
    MakiseStyleTheme *th = 0,
	*th_b = ((MToggle*)b->data)->state ?
	&((MToggle*)b->data)->style->active :
	&((MToggle*)b->data)->style->unactive;

    MToggle *t = ((MToggle*)b->data);
    if(t->focus_state == 0)
	th = &t->style->normal;
    else if(t->focus_state == 1)
	th = &t->style->focused;
    else if(t->focus_state >= 2)
    {
	th = &t->style->active;
	t->focus_state --;
    }
    _m_e_helper_draw_box(gui->buffer, &b->position, th);

    makise_d_string(gui->buffer,
		    t->text, MDTextAll,
		    b->position.real_x + 3,
		    b->position.real_y,
		    MDTextPlacement_LeftUp,
		    t->style->font,
		    th->font_col);

    makise_d_rect_filled(gui->buffer,
			 (MPoint){ b->position.real_x +
                                 b->position.width -
                                 b->position.height,
                                 b->position.real_y },
			 b->position.height, b->position.height,
                         th->thickness,
			 th->border_c,
			 th->bg_color);

    makise_d_rect_filled(gui->buffer,
			 (MPoint){ b->position.real_x +
                                 b->position.width -
                                 b->position.height + 2,
                                 b->position.real_y + 2},
			 b->position.height-4, b->position.height-4,
                         th_b->thickness,
			 th_b->border_c,
			 th_b->bg_color);
    
    //printf("Toggle %d dr\n", b->id);
    return M_OK;
}

MInputResultEnum input  (MElement* b, MInputData data)
{
    //printf("but %d inp %d %d\n", b->id, data.key, data.event);
    MToggle *e = ((MToggle*)b->data);

    if((data.key == M_KEY_OK
#if MAKISE_GUI_INPUT_POINTER_ENABLE == 1
	|| data.key == M_KEY_CURSOR
#endif
	   )
       && data.event == M_INPUT_CLICK)
    {
	e->state = !e->state;
	if(e->toggled != 0)
	    e->toggled(e, e->state);
	e->focus_state = 2;
    MAKISE_DEBUG_OUTPUT("cli %d\n", e->state);
	return M_INPUT_HANDLED;
    }
    return M_INPUT_NOT_HANDLED;
}
MFocusEnum focus  (MElement* b, MFocusEnum act)
{
    //MToggle *e = ((MToggle*)b->data);
    if(act & M_G_FOCUS_GET)
    {
	((MToggle*)b->data)->focus_state = 1;
    }
    if(act == M_G_FOCUS_LEAVE)
    {
	((MToggle*)b->data)->focus_state = 0;
    }
    //printf("but %d foc %d\n", b->id, act);
    return (act == M_G_FOCUS_PREV || act == M_G_FOCUS_NEXT)
	? M_G_FOCUS_NOT_NEEDED
	: M_G_FOCUS_OK;
}

#ifdef __cplusplus
}
#endif

#endif
