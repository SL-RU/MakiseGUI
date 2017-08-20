#include "makise_config.h"

#if ( MAKISE_E_SLIST > 0 )

#ifdef __cplusplus
extern "C" {
#endif

#include "makise_e_play_list.h"

static uint8_t              draw    ( MElement* b );
static MFocusEnum           focus   ( MElement* b,   MFocusEnum act );
static MInputResultEnum     input   ( MElement* b,   MInputData data );

static char* name = "PlayList";

//**********************************************************************
// Public functions ( element ).
//**********************************************************************
void m_create_play_list ( MPlayList*                obj_struct,
                          MContainer*               container,
                          MPosition                 pos,
                          char*                     header_text,
                          MPlayList_CallbackFunc*   user_func,
                          MakiseStyle_PlayList*     style,
                          MakiseStyle_PlayListItem* item_style ) {
    MElement *e = &obj_struct->e;
    m_element_create ( e, ( container == NULL ) ? 0 : container->gui,
                       name, obj_struct,
                       1, 1, pos,
                       &draw,
                       0,
                       0,
                       &input,
                       &focus,
                       0, 0);

    obj_struct->header_text             = header_text;
    obj_struct->f_array                 = user_func;
    obj_struct->style                   = style;
    obj_struct->item_style              = item_style;

    obj_struct->item_list               = NULL;
    obj_struct->is_array                = 0;
    obj_struct->len                     = 0;

    obj_struct->selected                = NULL;
    obj_struct->state                   = 0;

#if ( MAKISE_GUI_INPUT_POINTER_ENABLE == 1 )
    obj_struct->started                 = 0;
    obj_struct->sx                      = 0;
    obj_struct->sy                      = 0;
    obj_struct->sitem                   = NULL;
#endif

    makise_g_cont_add (container, e );

#if ( MAKISE_ENABLE_DEBUG_OUTPUT > 0 )
    MAKISE_DEBUG_OUTPUT( "PlayList %d created\n", e->id );
#endif

    return;
}

// Add one item to the list at the end. Only if NOT is_array.
void m_play_list_add ( MPlayList *obj, MPlayList_Item *item ) {

}

// Clear all pointers.
void m_play_list_clear ( MPlayList *obj ) {
    obj->selected   = 0;
    obj->is_array   = 0;
    obj->items      = 0;
}

// Remove item from linked list. Only if NOT is_array.
void m_play_list_remove     ( MPlayList *obj, MPlayList_Item *item ) {

}

// Set new data source. Simple array.
void m_play_list_set_array  ( MPlayList *obj, MPlayList_Item *array, uint32_t len ) {

}

// Set linked list as new data source.
void m_play_list_set_list   ( MPlayList *obj, MPlayList_Item *first ) {

}

//**********************************************************************
// Private functions ( element ).
//**********************************************************************


//**********************************************************************
// Private functions ( system ).
//**********************************************************************
static uint8_t draw ( MElement* b ) {

}

static MFocusEnum focus ( MElement* b, MFocusEnum act ) {

}

static MInputResultEnum input ( MElement* b, MInputData data ) {

}

#ifdef __cplusplus
}
#endif

#endif
