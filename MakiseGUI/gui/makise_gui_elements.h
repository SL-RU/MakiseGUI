#ifndef _MAKISE_H_G_EL
#define _MAKISE_H_G_EL 1

#ifdef __cplusplus
extern "C" {
#endif

#include "makise_config.h"    
    
typedef struct _MElement MElement;

#define MState_Disable 0b0000
#define MState_Idle    0b0001
#define MState_Focused 0b0010
#define MState_Pressed 0b0100

typedef enum  {
    MPositionAnchor_LeftUp,
    MPositionAnchor_RightUp,
    MPositionAnchor_LeftDown,
    MPositionAnchor_RighDown
} MPositionAnchor;

typedef enum {
    MPositionStretch_LeftRight,         // Stretch in horizontal direction.
    MPositionStretch_Left,              // No stretch. anchor is left.
    MPositionStretch_Right,             // No stretch. anchor is rigth.
} MPositionStretchHor;

typedef enum {
    MPositionStretch_UpDown,            // Stretch in vertical direction.
    MPositionStretch_Up,                // No stretch. anchor is up.
    MPositionStretch_Down,              // No stretch. anchor is down.
} MPositionStretchVert;

typedef enum {
    MFocusPrior_NotFocusble,            // Focus doesn't required
    MFocusPrior_Focusble,               // Focus is required
    MFocusPrior_FocusbleIsolated,       // Focus is required but only directly by M_G_FOCUS_GET, not _NEXT or _PREV.
} MFocusPriorEnum;

typedef struct _MPosition {
    MPositionStretchHor     horisontal;
    MPositionStretchVert    vertical;
    
    int32_t                 left;
    int32_t                 right;
    int32_t                 up;
    int32_t                 down;
    
    uint32_t                width;
    uint32_t                height;

    // It will be calculated automatically
    int32_t                 real_x;  // Position on the screen. It will be calculated automatically
    int32_t                 real_y;  // Position on the screen.It will be calculated automatically
} MPosition;

#include "makise_gui.h"

typedef struct _MElement {
    MPosition               position;       // Relative position of the element.
    
    void*                   data;

    MResult          (*draw      ) (MElement* el, MakiseGUI *gui);
    MResult          (*predraw   ) (MElement* el, MakiseGUI *gui); // Count real position for every element
    MResult          (*update    ) (MElement* el );
    MInputResultEnum (*input     ) (MElement* el, MInputData data);
    MFocusEnum       (*focus     ) (MElement* el, MFocusEnum act);
    
    uint8_t          is_parent;      // Is element parent(contains other elements.
    MContainer*      children;       // Only if element is parent.
    MElement*        prev;           // Previous if exists.
    MElement*        next;           // Next element if exists.
    MContainer*      parent;         // Parent element, if exists.
    MHost*           host;           // MHost, if exists.
    uint8_t          enabled;        // If enabled - methods will be executed.
    MFocusPriorEnum  focus_prior;    // Defines focus behavior
    uint32_t         id;             // Unique id.
    char*            name;
    
} MElement;

/**
 * Initialize MElement structure
 *
 * @param e Pointer to element themself
 * @param name Human name of the element
 * @param data pointer to structure with your data
 * @param enabled is element enabled
 * @param focus_prior focus prior
 * @param position MPosition
 * @param draw draw function
 * @param predraw predraw function
 * @param update update function
 * @param input input function
 * @param focus focus function
 * @param is_parent 1 if element is container or parent, else 0
 * @param children if is_parent==1 then MContainer, else 0
 * @return 
 */
void m_element_create(MElement *e, char *name, void* data,
		      uint8_t enabled, MFocusPriorEnum focus_prior,
		      MPosition position,
		      MResult    (*draw    )(MElement* el, MakiseGUI *gui),
		      MResult    (*predraw )(MElement* el, MakiseGUI *gui),
		      MResult    (*update  )(MElement* el),
		      MInputResultEnum (*input)(MElement* el, MInputData data),
		      MFocusEnum (*focus   )(MElement* el, MFocusEnum act),
		      uint8_t  is_parent,
		      MContainer *children);
/**
 * Lock element's mutex
 *
 * @param el element
 * @return result
 */
MResult m_element_mutex_request(MElement* el);
/**
 * Unlock element's mutex
 *
 * @param el element
 * @return result
 */
MResult m_element_mutex_release(MElement* el);
    
uint8_t m_element_call(MElement* el,  MakiseGUI *host, MElementCall type);
MInputResultEnum m_element_input(MElement* el, MInputData data);
MFocusEnum       m_element_focus(MElement* el, MFocusEnum act );    

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

/**
 * Create MPosition fully customizable. Unnecessary options you can equal to zero.
 *
 * @param left dist from left side. Required when hor is MPositionStretch_Left or MPositionStretch_LeftRight
 * @param right dist form the right side. Required when hor is MPositionStretch_Right or MPositionStretch_LeftRight
 * @param w width. Required when when hor is MPositionStretch_Left or MPositionStretch_Right
 * @param hor Type of horizontal stretch
 * @param up dist from top. Required when vert is MPositionStretch_Up or MPositionStretch_UpDown
 * @param down dist from bottom. Required when vert is MPositionStretch_Down or MPositionStretch_UpDown
 * @param h height. Required when vert is MPositionStretch_Up or MPositionStretch_Down
 * @param vert Type of vertical stretch
 * @return 
 */
MPosition mp_cust(int32_t left, int32_t right, uint32_t w,  MPositionStretchHor hor,
          int32_t up, int32_t down, uint32_t h, MPositionStretchVert vert);

/**
 * Create MPosition with horizontal stretch and vertical relative coordinates
 *
 * @param left dist from the left side of parent container to the left border
 * @param right dist from the right side of parent container to right border 
 * @param up dist from the top of the parent to the top border
 * @param h height
 * @return 
 */
MPosition mp_shor(int32_t left, int32_t right, int32_t up, uint32_t h);

/**
 * Create MPosition with horizontal relative coordinates and vertical stretch
 *
 * @param left left dist from the left side of parent container to the left border
 * @param width width
 * @param up dist from the top of the parent to the top border
 * @param down dist from the bottom of the parent to the bottom border
 * @return 
 */
MPosition mp_sver(int32_t left, int32_t width, int32_t up, uint32_t down);

/**
 * Create MPosition with horizontal and vertical stretch
 *
 * @param left left dist from the left side of parent container to the left border
 * @param right  dist from the right side of parent container to right border
 * @param up dist from the top of the parent to the top border
 * @param down dist from the bottom of the parent to the bottom border
 * @return 
 */
MPosition mp_sall(int32_t left, int32_t right, int32_t up, uint32_t down);

/**
 * Zero position
 *
 * @return 
 */
MPosition mp_nil();

#ifdef __cplusplus
}
#endif

#endif
