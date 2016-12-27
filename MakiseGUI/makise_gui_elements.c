#include "makise_gui_elements.h"

void m_button_create(MButton* b, MContainer *c,
			 int32_t x, int32_t y, uint32_t w, uint32_t h,
			 char* text,
			 uint32_t bgcolor, uint32_t fontcolor, uint32_t bordercolor)
{
    MElement *e = &b->el;
    e->gui = c->el->gui;

    e->draw = &_m_button_draw;
    e->predraw = &_m_button_predraw;
    e->update = 0;
    e->input = &_m_button_input;
    e->focus = &_m_button_focus;
    e->free = &_m_button_free;

    e->position.x = x;
    e->position.y = y;
    e->position.width = w;
    e->position.height = h;

    e->enabled = 1;
    e->id = makise_gui_newid();

    b->text = text;
    b->bgcolor = bgcolor;
    b->fontcolor = fontcolor;
    b->bordercolor = bordercolor;
    
}

uint8_t _m_button_draw   (MElement* b);
uint8_t _m_button_predraw(MElement* b);
uint8_t _m_button_input  (MElement* b, uint8_t event, uint32_t i);
uint8_t _m_button_focus  (MElement* b, uint8_t event);
uint8_t _m_button_free   (MElement* b);
