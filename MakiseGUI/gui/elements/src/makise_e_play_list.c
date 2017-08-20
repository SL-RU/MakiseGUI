#include "makise_config.h"

#if ( MAKISE_E_SLIST > 0 )

#ifdef __cplusplus
extern "C" {
#endif

#include "makise_e_play_list.h"

//**********************************************************************
// Private functions ( Prototype ).
//**********************************************************************
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
    if ( obj->is_array )  return;
    if ( obj->item_list == NULL ) {                 // Add first item.
        item->prev          = 0;
        item->next          = 0;
        obj->item_list      = item;
        obj->len            = 1;
        obj->selected       = item;
        return;
    }

    MPlayList_Item *it = obj->item_list;

    while ( it->next )
        it = it->next;

    it->next    = item;
    item->next  = 0;
    item->prev  = it;
    obj->len++;
}

// Clear all pointers.
void m_play_list_clear ( MPlayList *obj ) {
    obj->selected   = 0;
    obj->is_array   = 0;
    obj->item_list  = 0;
}

// Remove item from linked list. Only if NOT is_array.
void m_play_list_remove ( MPlayList *obj, MPlayList_Item *item ) {
    if ( obj == 0 || item == 0 || obj->item_list == 0 ) return;

    if ( obj->item_list == item ) {             // If first element.
        if ( item->next == 0 ) {
            m_play_list_clear( obj );
            return;
        }

        obj->item_list          = obj->item_list->next;
        obj->item_list->prev    = 0;
        item->next              = 0;
        item->prev              = 0;
        obj->selected           = obj->item_list;
        obj->len--;
        return;
    }

    if ( item->prev == 0 ) return;          // WTF?... Bag!

    if ( item->next == 0 ) {                // If last element.
        obj->len--;

        if ( item == obj->selected )
            obj->selected = item->prev;

        item->prev->next    = NULL;
        item->prev          = NULL;

        return;
    }

    if( item == obj->selected )
        obj->selected = item->prev;

    item->next->prev        = item->prev;
    item->prev->next        = item->next;
    item->prev              = NULL;
    item->next              = NULL;
    obj->len--;
}

// Set new data source. Simple array.
void m_play_list_set_array ( MPlayList *obj, MPlayList_Item *array, uint32_t len ) {

}

// Set linked list as new data source.
void m_play_list_set_list ( MPlayList *obj, MPlayList_Item *first ) {
    obj->item_list  = first;
    obj->selected   = first;
    obj->is_array   = 0;
    obj->len        = 0;
    if ( first == NULL ) return;

    MPlayList_Item* lst = first;
    while ( lst->next != NULL ) {
        obj->len++;
        lst = lst->next;
    }
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
