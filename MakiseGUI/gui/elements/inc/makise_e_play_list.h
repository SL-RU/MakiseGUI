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

} MPlayList_Item;

typedef struct {

} MPlayList;

typedef struct {
    void        ( *item_selected )     ( MPlayList *obj, MPlayList_Item* selected_item );
    void        ( *item_click )        ( MPlayList *obj, MPlayList_Item* selected_item );
    uint32_t    ( *get_item_count )    ( char* dir );
} MPlayList_CallbackFunc;

void m_create_play_list ( MPlayList*                b,
                          MContainer*               c,
                          MPosition                 pos,
                          char*                     text,
                          MPlayList_CallbackFunc*   user_func,
                          MakiseStyle_PlayList*     style,
                          MakiseStyle_PlayListItem* item_style);

#ifdef __cplusplus
}
#endif

#endif

#endif
