#include "makise_gui_elements.h"

void m_element_create(MElement *e, char *name, void* data,
		      uint8_t enabled, MFocusPriorEnum focus_prior,
		      MPosition position,
		      MResult    (*draw      )(MElement* el, MakiseGUI *gui),
		      MResult    (*predraw   )(MElement* el, MakiseGUI *gui),
		      MResult    (*update    )(MElement* el),
		      MInputResultEnum (*input   )(MElement* el, MInputData data),
		      MFocusEnum (*focus   )(MElement* el, MFocusEnum act),
		      uint8_t  is_parent,
		      MContainer *children)
{
    //clear structure
    memset(e, 0, sizeof(MElement));

    e->id = makise_g_newid();
    e->name                 = name;

    e->data                 = data;
    e->enabled              = enabled;
    e->focus_prior          = focus_prior;
    e->position             = position;
    e->draw                 = draw;
    e->predraw              = predraw;
    e->update               = update;
    e->input                = input;
    e->focus                = focus;
    e->is_parent            = is_parent;
    e->children             = children;

    e->next                 = 0;
    e->prev                 = 0;
    e->parent               = 0;
    e->host                 = 0;
}

MResult m_element_mutex_request(MElement* el)
{
    if(el == 0 || el->host == 0)
	return M_ERROR;
    MAKISE_MUTEX_REQUEST(el->host->mutex);
    return M_OK;
}
MResult m_element_mutex_release(MElement* el)
{
    if(el == 0 || el->host == 0)
	return M_ERROR;
    MAKISE_MUTEX_RELEASE(el->host->mutex);
    return M_OK;
}

uint8_t m_element_call(MElement* el, MakiseGUI *gui, MElementCall type)
{
    if(el == 0)
	return M_ZERO_POINTER;
    
    uint8_t result = M_ERROR;
    
    if(type == M_G_CALL_DRAW && el->draw != 0)
    {
	MakiseBufferBorderData d = makise_add_border(gui->buffer,
						     (MakiseBufferBorder){
							 el->position.real_x,
							     el->position.real_y,
							     el->position.width,
							     el->position.height,
							     0, 0});
	result = el->draw(el, gui);
	makise_rem_border(gui->buffer, d);
    }
    if(type == M_G_CALL_PREDRAW && el->predraw != 0)
    {
 	result = el->predraw(el, gui);
    }
    if(type == M_G_CALL_UPDATE && el->update != 0)
    {
	result = el->update(el);
    }
    return result;
}

MInputResultEnum m_element_input(MElement* el, MInputData data)
{
    if(el == 0)
	return M_INPUT_ERROR;

    MInputResultEnum r = M_INPUT_NOT_HANDLED;
    if(el->input != 0)
    {
	r = el->input(el, data);
    }
    
    return r;
}

MFocusEnum m_element_focus(MElement* el, MFocusEnum act )
{
    if(el == 0)
	return M_G_FOCUS_ERROR;
    
    MFocusEnum r = M_G_FOCUS_NOT_NEEDED;
    if(el->focus != 0)
    {
	r = el->focus(el, act);
    }

    return r;
}

MPosition mp_rel (int32_t x, int32_t y, uint32_t w, uint32_t h) {
    return mp_anc(x, y, w, h, MPositionAnchor_LeftUp);
}

MPosition mp_anc( int32_t x, int32_t y, uint32_t w, uint32_t h, MPositionAnchor anchor ) {
    MPosition p;

    switch ( anchor ) {
    case MPositionAnchor_LeftUp: 
        p.left = x;
        p.up = y;
        p.horisontal    = MPositionStretch_Left;
        p.vertical      = MPositionStretch_Up;
        break;
    case MPositionAnchor_LeftDown: 
        p.left = x;
        p.down = y;
        p.horisontal    = MPositionStretch_Left;
        p.vertical      = MPositionStretch_Down;
        break;
    case MPositionAnchor_RightUp: 
        p.right = x;
        p.up = y;
        p.horisontal    = MPositionStretch_Right;
        p.vertical      = MPositionStretch_Up;
        break;
    case MPositionAnchor_RighDown:
        p.right = x;
        p.down = y;
        p.horisontal    = MPositionStretch_Right;
        p.vertical      = MPositionStretch_Down;
        break;
    }

    p.width = w;
    p.height = h;
    return p;
}
MPosition mp_cust(int32_t left, int32_t right, uint32_t w,  MPositionStretchHor hor,
		  int32_t up, int32_t down, uint32_t h, MPositionStretchVert vert)
{
    MPosition p;
    p.left = left;
    p.right = right;
    p.up = up;
    p.down = down;
    p.horisontal = hor;
    p.vertical = vert;
    p.width = w;
    p.height = h;
    return p;
}

MPosition mp_shor(int32_t left, int32_t right, int32_t up, uint32_t h)
{
    MPosition p;
    p.left = left;
    p.right = right;
    p.up = up;
//    p.down = down;
    p.horisontal = MPositionStretch_LeftRight;
    p.vertical = MPositionStretch_Up;
//    p.width = w;
    p.height = h;
    return p;
}
MPosition mp_sver(int32_t left, int32_t w, int32_t up, uint32_t down)
{
    MPosition p;
    p.left = left;
//    p.right = right;
    p.up = up;
    p.down = down;
    p.horisontal = MPositionStretch_Left;
    p.vertical = MPositionStretch_UpDown;
    p.width = w;
//    p.height = h;
    return p;

}
MPosition mp_sall(int32_t left, int32_t right, int32_t up, uint32_t down)
{
    MPosition p;
    p.left = left;
    p.right = right;
    p.up = up;
    p.down = down;
    p.horisontal = MPositionStretch_LeftRight;
    p.vertical = MPositionStretch_UpDown;
//    p.width = w;
//    p.height = h;
    return p;

}
MPosition mp_nil()
{
    MPosition p;
    p.left = 0;
    p.up = 0;
    p.height = 0;
    p.width = 0;
    p.horisontal = MPositionStretch_Left;
    p.vertical = MPositionStretch_Up;
    return p;
}
