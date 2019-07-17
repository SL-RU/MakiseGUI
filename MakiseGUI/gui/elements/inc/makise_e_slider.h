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
    MakiseGUI*                gui;
    MElement                  el;

    int32_t*                  value_p;      //pointer to value
    int32_t                   value;        //value
    int32_t                   value_max;    //minimal possible value
    int32_t                   value_min;    //max possible value

    MSlider_Type              type;         //is slider readonly or you can control it

    void (*onchange)   (MSlider* b, int32_t val );    // When value changed by user
    void (*onfocus )   (MSlider* b, MFocusEnum type); // type == M_G_FOCUS_GET when focus recieved and M_G_FOCUS_LEAVE when dropped.

    const MakiseStyle*        style;
    uint8_t                   state; //focus state
} MSlider;

void m_create_slider( MSlider*                b,
                      MContainer*             c,
                      MPosition               pos,
                      MSlider_Type            type,
                      const MakiseStyle*      style );

void m_slider_set_range(MSlider* b, int32_t i, int32_t j);
void    m_slider_set_value        (MSlider* b, int32_t v);
//void    m_slider_set_value_pointer(MSlider* b, int32_t*v);    
int32_t m_slider_get_value(MSlider* b);
void m_slider_set_onchange(MSlider* b, void (*onchange)(MSlider* b, int32_t val));
void m_slider_set_onfocus (MSlider* b, void (*onfocus )(MSlider* b, MFocusEnum type));


    
#ifdef __cplusplus
}
#endif

#endif

#endif
