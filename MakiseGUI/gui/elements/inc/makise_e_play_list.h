#ifndef MAKISE_E_PLAY_LIST_H
#define MAKISE_E_PLAY_LIST_H        1

#include "makise_config.h"

#if ( MAKISE_E_PLAY_LIST > 0 )

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
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


    MakiseStyleTheme_PlayList   theme;
} MakiseStyle_PlayList;

typedef struct {
    const MakiseFont*           font;
    uint32_t                    font_line_spacing;

    MakiseStyleTheme_PlayList   normal;
    MakiseStyleTheme_PlayList   play;
    MakiseStyleTheme_PlayList   selected;
} MakiseStyle_PlayListItem;

// [256]
// [ 2 + 1 + 2 + 1 + 2 + 1 ]
typedef struct _MPlayList_Item MPlayList_Item;
typedef struct _MPlayList_Item{
    char*                       name_string;                    // 255 char + 1 zero (256 char string).
    char*                       time_string;                    // Max: 99:99:59\0. HH:MM:SS\0 (9 char string).
                                                                // Example: 1:20:0\0 1:14:2\0 1:08\0.
    uint32_t                    time_sec;
    uint32_t                    real_number_track;              // The track number in your list. 0..MAX_UINT32_T.
    MPlayList_Item*             prev;
    MPlayList_Item*             next;

    uint8_t                     play_state;                     // 1 - treck selected how play. 0 - no.
    uint8_t                     selected_state;                 // 1 - treck selected how selected. 0 - no.
    uint32_t                    id;
} MPlayList_Item;

typedef struct _MPlayList MPlayList;
typedef struct {
    MPlayList_Item* ( *create_array_item )          ( uint32_t len );
    void            ( *item_selected )              ( MPlayList_Item* selected_item );
    uint8_t         ( *item_click )                 ( MPlayList_Item* click_item );
    void            ( *get_item_name_and_time )     ( MPlayList_Item* selected_item, uint32_t treck_number );
    uint32_t        ( *get_file_count_of_dir)       ( char* dir );
} MPlayList_CallbackFunc;

typedef struct _MPlayList {
    MElement                    e;
    char*                       header_text;

    MPlayList_Item*             item_array;
    uint32_t                    item_array_len;
    uint32_t                    eh;             // Height one string list for lcd.
    MPlayList_Item*             selected;

#if MAKISE_GUI_INPUT_POINTER_ENABLE == 1
    uint8_t                     started;        // Was pressing already started
    int32_t                     sx;             // cursor's x when pressing was started.
    int32_t                     sy;
    MPlayList_Item*             sitem;          // Focused item when pressing was started.
#endif

    MPlayList_CallbackFunc*     f_array;            // User function.

    MakiseStyle_PlayList*       style;
    MakiseStyle_PlayListItem*   item_style;

    char*                       current_dir;
    uint32_t                    file_count_of_dir;
    uint32_t                    focus_file_number;  // Focus state.
    int32_t                     play_file_number;   // -1 == no play file.
} MPlayList;

void m_create_play_list     ( MPlayList*                obj_struct,
                              MContainer*               container,
                              MPosition                 pos,
                              char*                     header_text,
                              char*                     current_dir,
                              MPlayList_CallbackFunc*   user_func,
                              MakiseStyle_PlayList*     style,
                              MakiseStyle_PlayListItem* item_style );

void m_click_play_list      ( MPlayList* b, MInputKeyEnum key );

/*
// Update data item.
void m_play_list_item_data_update ( MPlayList_Item* item, char* name_track, uint32_t time_sec ) {
    strcpy( item->name, );
}

// Set count item in directory.
void m_play_list_set_count_item_in_dir ( uint32_t count_item ) {

}
  */
#ifdef __cplusplus
}
#endif

#endif

#endif
