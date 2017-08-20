#ifndef MAKISE_E_PLAY_LIST_H
#define MAKISE_E_PLAY_LIST_H        1

#include "makise_config.h"

#if ( MAKISE_E_PLAY_LIST > 0 )

#ifdef __cplusplus
extern "C" {
#endif

#include "makise_e.h"

typedef struct {
    uint32_t bg_color;
    uint32_t font_col;
    uint32_t border_c;

    uint16_t double_border;
} MakiseStyleTheme_PlayList;

typedef struct {
    const MakiseFont*           font;
    uint32_t                    font_line_spacing;

    // Spacing set.
    uint8_t                     left_margin;
    uint8_t                     item_margin;     // Vertical.

    // Scroll.
    uint8_t                     scroll_width;
    uint32_t                    scroll_bg_color; // May be 0.
    uint32_t                    scroll_color;


    MakiseStyleTheme_PlayList  normal;
    MakiseStyleTheme_PlayList  focused;
    MakiseStyleTheme_PlayList  active;
} MakiseStyle_PlayList;

typedef struct {
    const MakiseFont*           font;
    uint32_t                    font_line_spacing;

    MakiseStyleTheme_PlayList   normal;
    MakiseStyleTheme_PlayList   focused;
    MakiseStyleTheme_PlayList   active;
} MakiseStyle_PlayListItem;

typedef struct {
    char                        name[256];
    uint32_t                    len_itemsec;

    MPlayList_Item*             prev;
    MPlayList_Item*             next;

    uint32_t                    id;             // Custom id, if NOT is_array, else - position in the array (will be computed automatically by PlayList).
} MPlayList_Item;

typedef struct {
    void        ( *item_selected )     ( MPlayList *obj, MPlayList_Item* selected_item );
    void        ( *item_click )        ( MPlayList *obj, MPlayList_Item* selected_item );
    uint32_t    ( *get_item_count )    ( char* dir );
} MPlayList_CallbackFunc;

typedef struct {
    MakiseGUI*                  gui;
    MElement                    e;
    char*                       header_text;

    MPlayList_Item*             item_list;
    uint8_t                     is_array;       // If 1 then items will be
    uint32_t                    len;            // matters only if is_array.

    MPlayList_Item*             selected;
    uint32_t                    state;          // Focus state.

#if MAKISE_GUI_INPUT_POINTER_ENABLE == 1
    uint8_t                     started;        // Was pressing already started
    int32_t                     sx;             // cursor's x when pressing was started.
    int32_t                     sy;
    MPlayList_Item*             sitem;          // Focused item when pressing was started.
#endif

    MPlayList_CallbackFunc*     f_array;        // User function.

    MakiseStyle_PlayList*       style;
    MakiseStyle_PlayListItem*   item_style;
} MPlayList;



void m_create_play_list     ( MPlayList*                obj_struct,
                              MContainer*               container,
                              MPosition                 pos,
                              char*                     header_text,
                              MPlayList_CallbackFunc*   user_func,
                              MakiseStyle_PlayList*     style,
                              MakiseStyle_PlayListItem* item_style);

void m_play_list_add        ( MPlayList *obj, MPlayList_Item *item );                     // Add one item to the list at the end. Only if NOT is_array.
void m_play_list_clear      ( MPlayList *obj );                                           // Clear all pointers.
void m_play_list_remove     ( MPlayList *obj, MPlayList_Item *item );                     // Remove item from linked list. Only if NOT is_array.
void m_play_list_set_array  ( MPlayList *obj, MPlayList_Item *array, uint32_t len );      // Set new data source. Simple array.
void m_play_list_set_list   ( MPlayList *obj, MPlayList_Item *first );                    // Set linked list as new data source.

#ifdef __cplusplus
}
#endif

#endif

#endif
