#include "makise_gui_elements.h"

uint8_t m_element_call(MElement* el, uint8_t type)
{
    if(el == 0)
	return M_ZERO_POINTER;

    if(type == M_G_CALL_DRAW && el->draw != 0)
    {
	MakiseBufferBorderData d =makise_add_border(el->gui->buffer,
						    (MakiseBufferBorder){
							el->position.real_x,
							    el->position.real_y,
							    el->position.width,
							    el->position.height,
							    0, 0});
	uint8_t r = el->draw(el);
	makise_rem_border(el->gui->buffer, d);
	return r;
    }
    if(type == M_G_CALL_PREDRAW && el->predraw != 0)
	return el->predraw(el);
    if(type == M_G_CALL_UPDATE && el->update != 0)
	return el->update(el);

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

MPosition mp_rel(int32_t x, int32_t y, uint32_t w, uint32_t h)
{
    MPosition p;
    p.x = x;
    p.y = y;
    p.width = w;
    p.height = h;
    return p;
}
