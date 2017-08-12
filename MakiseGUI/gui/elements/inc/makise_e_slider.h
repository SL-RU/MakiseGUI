#ifndef MAKISE_E_SLIDER_H
#define MAKISE_E_SLIDER_H       1

#include "makise_config.h"

#if ( MAKISE_E_SLIDER > 0 )

#ifdef __cplusplus
extern "C" {
#endif

#include "makise_e.h"

typedef enum {
    MSlider_Type_Read,
    MSlider_Type_ReadWrite,
} MSlider_Type;

#define _MSlider_is_horizontal(x) (x->position.width >= x->position.height)

typedef struct _MSlider MSlider;

typedef struct _MSlider {
    MakiseGUI*          gui;
    MElement            el;

    int32_t*            value;                                                      //pointer to value
    int32_t             value_max;                                                  //minimal possible value
    int32_t             value_min;                                                  //max possible value

    MSlider_Type        type;                                                       //is slider readonly or you can control it

    void                ( *onchange )   ( MSlider* b, int32_t val );                 // When value changed by user
    void                ( *onfocus )    ( MSlider* b, MFocusEnum type );             // type == M_G_FOCUS_GET when focus recieved and M_G_FOCUS_LEAVE when dropped.

    MakiseStyle*        style;
    uint8_t             state;                                                       //focus state
} MSlider;

void m_create_slider( MSlider*          b,
                      MContainer*       c,
                      MPosition         pos,
                      int32_t*          value,
                      int32_t           value_min,
                      int32_t           value_max,
                      MSlider_Type      type,
                      void              ( *onchange )   ( MSlider* b, int32_t val ),
                      void              ( *onfocus )    ( MSlider* b, MFocusEnum type ),
                      MakiseStyle*      style );

#ifdef __cplusplus
}
#endif

#endif

#endif
