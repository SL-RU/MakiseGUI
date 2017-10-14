#include "makise_gui_elements.h"

void m_element_create(MElement *e, MakiseGUI *gui, char *name, void* data,
		      uint8_t enabled, uint8_t focus_prior,
		      MPosition position,
		      uint8_t    (*draw    )(MElement* el),
		      uint8_t    (*predraw )(MElement* el),
		      uint8_t    (*update  )(MElement* el),
		      MInputResultEnum (*input   )(MElement* el, MInputData data),
		      MFocusEnum (*focus   )(MElement* el, MFocusEnum act),
		      uint8_t  is_parent,
		      MContainer *children)
{
    e->id = makise_g_newid();
    
    e->gui                  = gui;
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
    
#if MAKISE_MUTEX
    m_mutex_create(&e->mutex);
    m_mutex_create(&e->mutex_cont);
#endif

}

uint8_t m_element_call(MElement* el, uint8_t type)
{
    if(el == 0)
	return M_ZERO_POINTER;
    
    MAKISE_MUTEX_REQUEST(&el->mutex);
    uint8_t result;
    
    if(type == M_G_CALL_DRAW && el->draw != 0)
    {
	MakiseBufferBorderData d =makise_add_border(el->gui->buffer,
						    (MakiseBufferBorder){
							el->position.real_x,
							    el->position.real_y,
							    el->position.width,
							    el->position.height,
							    0, 0});
	result = el->draw(el);
	makise_rem_border(el->gui->buffer, d);
	MAKISE_MUTEX_RELEASE(&el->mutex);
	return result;
    }
    if(type == M_G_CALL_PREDRAW && el->predraw != 0)
    {
 	result = el->predraw(el);
	MAKISE_MUTEX_RELEASE(&el->mutex);
	return result;
    }
    if(type == M_G_CALL_UPDATE && el->update != 0)
    {
	result = el->update(el);
	MAKISE_MUTEX_RELEASE(&el->mutex);
	return result;
    }
    MAKISE_MUTEX_RELEASE(&el->mutex);
    return M_ERROR;
}

uint8_t m_element_input(MElement* el, MInputData data)
{
    if(el == 0)
	return M_ZERO_POINTER;

    if(el->input != 0)
    {
	el->input(el, data);
    }
    
    return 0;
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
