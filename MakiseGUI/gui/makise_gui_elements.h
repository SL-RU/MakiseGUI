#ifndef _MAKISE_H_G_EL
#define _MAKISE_H_G_EL 1

typedef struct _MElement MElement;
typedef struct _MPosition MPosition;

#define MState_Disable 0b0000
#define MState_Idle    0b0001
#define MState_Focused 0b0010
#define MState_Pressed 0b0100

#include "makise_gui.h"
#include "makise_gui_container.h"

typedef enum
{
    MPositionAnchor_LeftUp,
    MPositionAnchor_RightUp,
    MPositionAnchor_LeftDown,
    MPositionAnchor_RighDown
} MPositionAnchor;

typedef enum
{
    MPositionStretch_LeftRight, //stretch in horizontal direction
    MPositionStretch_Left, //no stretch. anchor is left
    MPositionStretch_Right,//no stretch. anchor is rigth
} MPositionStretchHor;

typedef enum
{
    MPositionStretch_UpDown, //stretch in vertical direction
    MPositionStretch_Up, //no stretch. anchor is up
    MPositionStretch_Down,//no stretch. anchor is down
} MPositionStretchVert;

typedef struct _MPosition
{
    MPositionStretchHor horisontal;
    MPositionStretchVert vertical;
    
    int32_t left;  
    int32_t right; 
    int32_t up;    
    int32_t down;  
    
    uint32_t width;
    uint32_t height;

    //it will be calculated automatically
    int32_t real_x; //position on the screen
    int32_t real_y; //position on the screen
} MPosition;

typedef struct _MElement
{
    MakiseGUI *gui; //current gui

    uint32_t id; //unique id
    char* name;
    
    MElement *prev; //previous if exists
    MElement *next; //next element if exists
    MContainer *parent; //parent element, if exists

    uint8_t enabled;    //if enabled - methods will be executed
    MPosition position; //relative position of the element

    uint8_t focus_prior; //relative position in focus queu. 0 means focus doesn't required
    
    void* data;
    uint8_t    (*draw    )(MElement* el);
    uint8_t    (*predraw )(MElement* el); //count real position for every element
    uint8_t    (*update  )(MElement* el);
    MInputResultEnum (*input   )(MElement* el, MInputData data);
    MFocusEnum (*focus   )(MElement* el, MFocusEnum act);
    

    uint8_t  is_parent; //is element parent(contains other elements
    MContainer *children; //only if element is parent
    
} MElement;


uint8_t m_element_call(MElement* el, uint8_t type);
uint8_t m_element_input(MElement* el, MInputData data);

/**
 * Create MPosition from relative coordinates & size. Anchor - left up corner
 *
 * @param h 
 * @return 
 */
MPosition mp_rel(int32_t x, int32_t y, uint32_t w, uint32_t h);
/**
 * Create MPosition from relative coordinates and custom anchor
 *
 * @param x horizontal coordinate
 * @param y vertical coordinate
 * @param w width
 * @param h height
 * @param anchor  
 * @return 
 */
MPosition mp_anc(int32_t x, int32_t y, uint32_t w, uint32_t h, MPositionAnchor anchor);
#endif
