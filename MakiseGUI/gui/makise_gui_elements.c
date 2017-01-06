#include "makise_gui_elements.h"

uint8_t m_element_call(MElement* el, uint8_t type)
{
    if(type == M_G_CALL_DRAW)
	return el->draw(el);
    if(type == M_G_CALL_PREDRAW)
	return el->predraw(el);
    if(type == M_G_CALL_UPDATE)
	return el->update(el);

    return M_ERROR;
}
uint8_t m_element_focus(MElement* el, uint8_t event)
{
    return 0;
}
uint8_t m_element_input(MElement* el, uint8_t event, uint32_t i)
{
    return 0;
}
