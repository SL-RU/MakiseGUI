#include "makise_gui.h"

uint8_t _m_button_draw   (MElement* b);
uint8_t _m_button_predraw(MElement* b);
MInputResultEnum _m_button_input  (MElement* b, MInputData data);
MFocusEnum _m_button_focus  (MElement* b, MFocusEnum act);
uint8_t _m_button_free   (MElement* b);

void m_button_create(MButton* b, MContainer *c,
			 int32_t x, int32_t y, uint32_t w, uint32_t h,
			 char* text,
			 uint32_t bgcolor, uint32_t fontcolor, uint32_t bordercolor)
{
    MElement *e = &b->el;
    e->gui = c->gui;

    e->data = b;
    
    makise_g_cont_add(c, e);

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
    e->id = makise_g_newid();

    b->text = text;
    b->bgcolor = bgcolor;
    b->fontcolor = fontcolor;
    b->bordercolor = bordercolor;

    printf("Button %d created\n", e->id);
}

uint8_t _m_button_draw   (MElement* b)
{
    makise_d_rect_filled(b->gui->buffer, b->position.real_x, b->position.real_y, b->position.width, b->position.height, ((MButton*)b->data)->bordercolor, ((MButton*)b->data)->bgcolor);
    makise_d_line(b->gui->buffer, b->position.real_x, b->position.real_y, 0, 0, 3);
    makise_d_string(b->gui->buffer, ((MButton*)b->data)->text, b->position.real_x + 2, b->position.real_y, &F_Arial12, ((MButton*)b->data)->fontcolor);

    //printf("Button %d dr\n", b->id);
    return M_OK;
}
uint8_t _m_button_predraw(MElement* b)
{
//    printf("Button %d predr\n", b->id);
    return M_OK;
}
MInputResultEnum _m_button_input  (MElement* b, MInputData data)
{
}
MFocusEnum _m_button_focus  (MElement* b, MFocusEnum act)
{
}
uint8_t _m_button_free   (MElement* b)
{
}
