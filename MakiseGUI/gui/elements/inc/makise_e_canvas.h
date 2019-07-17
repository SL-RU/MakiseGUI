#ifndef MAKISE_E_CANVAS_H
#define MAKISE_E_CANVAS_H       1

#include "makise_config.h"

#if ( MAKISE_E_CANVAS > 0 )

#ifdef __cplusplus
extern "C" {
#endif

#include "makise_e.h"


typedef struct {
    MakiseStyleTheme normal;
    MakiseStyleTheme focused;
} MakiseStyle_Canvas;


//Canvas - simple container. It's placing elements simply by their position wherever it needed
typedef struct {
    MElement                el;
    MContainer              cont;
    MakiseStyle_Canvas*     style;

    MElement *last_focused;

    uint8_t                 state;
} MCanvas;

void m_create_canvas( MCanvas*              b,
                      MContainer*           c,
                      MPosition             pos,
                      MakiseStyle_Canvas*   style );


/**
 * Set container focus behavior. If isolated then focus won't go to parent's container, else will
 *
 * @param b container
 * @param isolated MContainer_Isolated or MContainer_NotIsolated
 * @return 
 */
void m_canvas_set_isolated(MCanvas* b,
			   MContainerIsolated_t isolated);

#ifdef __cplusplus
}
#endif

#endif

#endif
