#ifndef MAKISE_E_CANVAS_H
#define MAKISE_E_CANVAS_H       1

#include "makise_config.h"

#if ( MAKISE_E_CANVAS > 0 )

#ifdef __cplusplus
extern "C" {
#endif

#include "makise_e.h"


typedef struct {
    uint32_t bg_color;
    uint32_t border_c;

    uint16_t double_border;
} MakiseCanvasStyleTheme;

typedef struct {
    MakiseCanvasStyleTheme normal;
    MakiseCanvasStyleTheme focused;
} MakiseCanvasStyle;


//Canvas - simple container. It's placing elements simply by their position wherever it needed
typedef struct {
    MakiseGUI*              gui;
    MElement                el;

    MContainer              cont;

    MakiseCanvasStyle*      style;

    uint8_t                 state;
} MCanvas;

void m_create_canvas( MCanvas*            b,
                      MContainer*         c,
                      MPosition           pos,
                      MakiseCanvasStyle*  style );

#ifdef __cplusplus
}
#endif

#endif

#endif
