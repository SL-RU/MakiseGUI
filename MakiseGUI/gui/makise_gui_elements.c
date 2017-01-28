#include "makise_gui_elements.h"

uint8_t m_element_call(MElement* el, uint8_t type)
{
    if(el == 0)
	return M_ZERO_POINTER;

    if(type == M_G_CALL_DRAW)
	return el->draw(el);
    if(type == M_G_CALL_PREDRAW)
	return el->predraw(el);
    if(type == M_G_CALL_UPDATE)
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
