#ifndef MAKISE_E_H
#define MAKISE_E_H 1

#include "makise_gui_elements.h"
#include "makise_gui_container.h"

typedef struct {
    MakiseGUI *gui;
    MElement el;

    char* text;
    uint32_t bgcolor;
    uint32_t fontcolor;
    uint32_t bordercolor;
    
    uint8_t state;    
} MButton;

void m_button_create(MButton* b, MContainer *c,
		     int32_t x, int32_t y, uint32_t w, uint32_t h,
		     char* text,
		     uint32_t bgcolor, uint32_t fontcolor, uint32_t bordercolor);

typedef struct {
    MakiseGUI *gui;
    MElement el;

    MContainer cont;

    uint32_t bgcolor;
    uint32_t bordercolor;
    
    uint8_t state;    
} MCanvas;

void m_canvas_create(MCanvas* b, MContainer *c,
		     int32_t x, int32_t y, uint32_t w, uint32_t h,
		     uint32_t bgcolor, uint32_t bordercolor);


#endif
