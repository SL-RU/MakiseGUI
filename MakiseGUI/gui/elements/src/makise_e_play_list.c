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
static MInputResultEnum     input   ( MElement* b,   MInputData data );
static MFocusEnum           focus   ( MElement* b,   MFocusEnum act );

// Set new data source. Simple array.
void m_play_list_init_item_array ( MPlayList_Item *array, uint32_t len ) {
    MPlayList_Item *lst = NULL;

    for ( uint32_t i = 0; i < len; i++ ) {
        array[i].play_state = 0;
        array[i].selected_state = 0;
        array[i].prev = lst;
        array[i].next = ( ( i + 1 ) < len ) ? &array[ i + 1 ] : NULL;
        array[i].id = i;
        lst = &array[i];
    }
}

static char* name = "PlayList";

//**********************************************************************
// Public functions ( element ).
//**********************************************************************
void m_create_play_list ( MPlayList*                obj_struct,
                          MContainer*               container,
                          MPosition                 pos,
                          char*                     header_text,
                          char*                     current_dir,
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



    // Height one string list for lcd.
    obj_struct->eh                      = item_style->font->height +
                                          2 +        // 2 line board.
                                          2;         // 2 space line.

    // Count of elements on the screen. One line total.
    obj_struct->item_array_len          = pos.height;
    obj_struct->item_array_len         -= ( header_text != NULL ) ? obj_struct->style->font->height + 4 : 0;
    obj_struct->item_array_len         /= ( obj_struct->eh - 1 );


    obj_struct->item_array                  = user_func->create_array_item( obj_struct->item_array_len );
    m_play_list_init_item_array( obj_struct->item_array, obj_struct->item_array_len );
    obj_struct->selected                    = obj_struct->item_array;
    obj_struct->selected->selected_state    = 1;
    obj_struct->focus_file_number           = 0;
    obj_struct->play_file_number            = -1;       // No play track.

#if ( MAKISE_GUI_INPUT_POINTER_ENABLE == 1 )
    obj_struct->started                 = 0;
    obj_struct->sx                      = 0;
    obj_struct->sy                      = 0;
    obj_struct->sitem                   = NULL;
#endif

    // Get information about the directory.
    obj_struct->current_dir             = current_dir;
    obj_struct->file_count_of_dir       = obj_struct->f_array->get_file_count_of_dir( current_dir );

    uint32_t selected_track = 0;
    for ( uint32_t i_loop = 0; i_loop < obj_struct->item_array_len ; i_loop++ ) {
        obj_struct->f_array->get_item_name_and_time( &obj_struct->item_array[ i_loop ], selected_track );
        obj_struct->item_array[ i_loop ].real_number_track = selected_track;
        selected_track++;
    }

    makise_g_cont_add ( container, e );

#if ( MAKISE_ENABLE_DEBUG_OUTPUT > 0 )
    MAKISE_DEBUG_OUTPUT( "PlayList %d created\n", e->id );
#endif

    return;
}

//**********************************************************************
// Private functions ( element ).
//**********************************************************************
//draw line frome the list
static void draw_item ( MPlayList *obj, MPlayList_Item *pl_i, MakiseStyleTheme_PlayList *c_th, uint32_t x, uint32_t y, uint32_t w, uint32_t eh ) {
    makise_d_rect_filled( obj->e.gui->buffer,
                          x, y, w, eh,
                          c_th->border_c, c_th->bg_color);

    uint32_t time_width;
    time_width = makise_d_string_width( pl_i->time_string, MDTextAll, obj->item_style->font );

    makise_d_string_frame( obj->e.gui->buffer,                                                  // Draw time string.
                           pl_i->time_string, MDTextAll,
                           w - time_width - 1, y + 2,
                           time_width, eh,
                           obj->item_style->font,
                           obj->item_style->font_line_spacing,
                           c_th->font_col );

    makise_d_string_frame( obj->e.gui->buffer,
                           pl_i->name_string, MDTextAll,
                           x + 2, y + 2,
                           w - 4 - time_width, eh,
                           obj->item_style->font,
                           obj->item_style->font_line_spacing,
                           c_th->font_col );
}

static uint32_t get_selected_item_number ( const MPlayList *obj ) {
    return obj->selected->id;
}

static void header_text_draw ( const MPlayList* obj, const int16_t* x, int16_t* y, const uint32_t* height_block, uint32_t* free_h ) {
    if ( obj->header_text != NULL ) {
        MakiseStyleTheme_PlayList* th = &obj->style->theme;

        makise_d_string_frame( obj->e.gui->buffer,
                               obj->header_text, MDTextAll,
                               *x + 2,  // One line board + One line space.
                               *y + 2,  // One line board + One line space.
                               obj->e.position.width - 4, *height_block,
                               obj->item_style->font,
                               obj->item_style->font_line_spacing,
                               th->font_col );

        *y += obj->style->font->height + 4;         // Two line board + 2 line space.
        *free_h -= obj->style->font->height + 4;    // 2 pixel line (up and down) + 2 space (up and down).

        makise_d_line( obj->e.gui->buffer,
                       obj->e.position.real_x, *y,
                       obj->e.position.real_x + obj->e.position.width,
                       *y,
                       th->border_c);
    }
    return;
}

static void drawing_scroll ( const MPlayList* obj, uint32_t field_h, uint32_t field_y, const uint32_t y_pos, const uint32_t h_scroll ) {
    if ( obj->style->scroll_width != 0 ) {
        MakiseStyleTheme_PlayList* th = &obj->style->theme;

        makise_d_rect_filled( obj->e.gui->buffer,
                  obj->e.position.real_x + obj->e.position.width - obj->style->scroll_width,
                  field_y,
                  obj->style->scroll_width,
                  field_h,
                  th->border_c,
                  obj->style->scroll_bg_color );

        makise_d_rect_filled( obj->e.gui->buffer,
                  obj->e.position.real_x + obj->e.position.width - obj->style->scroll_width,
                  y_pos,
                  obj->style->scroll_width,
                  h_scroll,
                  th->border_c,
                  obj->style->scroll_color );
    }

}

static void array_reset_to_start ( const MPlayList* obj_struct ) {
    for ( uint32_t i_loop = 0; i_loop < obj_struct->item_array_len ; i_loop++ ) {
        obj_struct->f_array->get_item_name_and_time( &obj_struct->item_array[ i_loop ], i_loop );
        obj_struct->item_array[ i_loop ].real_number_track = i_loop;
    }
}

static void array_reset_to_end ( const MPlayList* obj_struct ) {
    if ( obj_struct->file_count_of_dir <= obj_struct->item_array_len ) {
        array_reset_to_start( obj_struct );
        return;
    }

    uint32_t p_track = obj_struct->file_count_of_dir - obj_struct->item_array_len;
    for ( uint32_t i_loop = 0; i_loop < obj_struct->item_array_len; i_loop++ ) {
        obj_struct->f_array->get_item_name_and_time( &obj_struct->item_array[ i_loop ], p_track );
        obj_struct->item_array[ i_loop ].real_number_track = p_track;
        p_track++;
    }
}

//**********************************************************************
// Private functions ( system ).
//**********************************************************************
static uint8_t draw ( MElement* b ) {
    MPlayList *obj = ( MPlayList* )b->data;

    MakiseStyleTheme_PlayList* st = &obj->style->theme;

    int16_t     y       = b->position.real_y;
    int16_t     x       = b->position.real_x  + obj->style->left_margin;
    uint32_t    w       = b->position.width   - obj->style->scroll_width;
                w      += ( obj->style->scroll_width != 0 ) ? 1 : 0;
    uint32_t    h       = b->position.height;

    _m_e_helper_draw_box_param( b->gui->buffer, &b->position, st->border_c, st->bg_color, st->double_border );
    header_text_draw( obj, &x, &y, &obj->eh, &h );                   // Draw header.

    for ( uint32_t i = 0; i < obj->item_array_len; i++ ) {
        if ( obj->item_array[ i ].selected_state == 1 ) {
            st = &obj->item_style->selected;
        } else if ( obj->item_array[ i ].play_state == 1 ) {
            st = &obj->item_style->play;
        } else {
            st = &obj->item_style->normal;
        }

        draw_item( obj, &obj->item_array[ i ], st, x, y, w, obj->eh );

        y += obj->eh - 1;
    }

    // If overlap occurs, the frame is more important.
    st = &obj->style->theme;
    makise_d_rect( b->gui->buffer,
                   b->position.real_x,
                   b->position.real_y,
                   b->position.width,
                   b->position.height,
                   st->border_c );

    if ( obj->style->scroll_width == 0 ) return M_OK;

    uint32_t y_scroll = obj->e.position.real_y;

    if ( obj->header_text != NULL ) {
        y_scroll += obj->style->font->height + 4;
    }

    uint32_t field_height   = obj->e.position.height;
    uint32_t scroll_height  = ( field_height - 2 ) / obj->file_count_of_dir;                // 2 line for line frame.
    drawing_scroll( obj, field_height, y_scroll, y_scroll + scroll_height * obj->selected->real_number_track + 1, scroll_height );

    return M_OK;
}

static void update_play_state ( MPlayList *obj ) {
    for ( uint32_t i = 0; i < obj->item_array_len; i++ ) {
        if ( obj->item_array[ i ].real_number_track == obj->play_file_number ) {
            obj->item_array[ i ].play_state = 1;
        } else {
            obj->item_array[ i ].play_state = 0;                                        // Clear old pos track play.
        }
    }
}

void m_click_play_list ( MPlayList* b, MInputKeyEnum key ) {
    MInputData  in_data;
    in_data.key         = key;
    in_data.event       = M_INPUT_CLICK;
    input( &b->e, in_data );
}

static MInputResultEnum input ( MElement* b, MInputData data ) {
    if ( data.event != M_INPUT_CLICK ) return M_INPUT_NOT_HANDLED;
    MPlayList *obj = ( MPlayList* )b->data;
    switch ( data.key ) {
    case M_KEY_DOWN:
        // Move the scroll in the visible part of the screen.
        if ( obj->selected->id < obj->item_array_len - 1 ) {
            obj->selected->selected_state = 0;
            obj->selected = obj->selected->next;
            obj->selected->selected_state = 1;

            update_play_state( obj );
            return M_INPUT_NOT_HANDLED;
        }

        // Shift the elements up. The upper one is lost.
        if ( obj->selected->real_number_track < obj->file_count_of_dir - 1 ) {
            char* b_char_name = obj->item_array[ 0 ].name_string;
            char* b_char_time = obj->item_array[ 0 ].time_string;
            uint32_t time_sec   = obj->item_array[ 0 ].time_sec;

            for ( uint32_t p_l = 1; p_l < obj->item_array_len; p_l++ ) {
                obj->item_array[ p_l - 1 ].name_string                 = obj->item_array[ p_l ].name_string;
                obj->item_array[ p_l - 1 ].time_string                 = obj->item_array[ p_l ].time_string;
                obj->item_array[ p_l - 1 ].real_number_track           = obj->item_array[ p_l ].real_number_track;
                obj->item_array[ p_l - 1 ].time_sec                    = obj->item_array[ p_l ].time_sec;
            }

            obj->item_array[ obj->item_array_len - 1 ].name_string = b_char_name;
            obj->item_array[ obj->item_array_len - 1 ].time_string = b_char_time;
            obj->item_array[ obj->item_array_len - 1 ].time_sec    = time_sec;

            obj->f_array->get_item_name_and_time( &obj->item_array[ obj->item_array_len - 1 ], obj->selected->real_number_track + 1 );
            obj->item_array[ obj->item_array_len - 1 ].real_number_track = obj->selected->real_number_track + 1;

            update_play_state( obj );
            return M_INPUT_NOT_HANDLED;
        }

        // Go to the beginning.
        obj->selected->selected_state = 0;
        array_reset_to_start( obj );
        obj->selected = obj->item_array;
        obj->selected->selected_state = 1;

        update_play_state( obj );
        return M_INPUT_NOT_HANDLED;

    case M_KEY_UP:
        if ( obj->selected != obj->item_array ) {
            obj->selected->selected_state = 0;
            obj->selected = obj->selected->prev;
            obj->selected->selected_state = 1;

            update_play_state( obj );
            return M_INPUT_NOT_HANDLED;
        }

        if ( obj->selected->real_number_track != 0 ) {
            char* b_char_name = obj->item_array[ obj->item_array_len - 1 ].name_string;
            char* b_char_time = obj->item_array[ obj->item_array_len - 1 ].time_string;
            uint32_t time_sec = obj->item_array[ obj->item_array_len - 1 ].time_sec;

            for ( uint32_t p_l = obj->item_array_len - 1; p_l > 0; p_l-- ) {
                obj->item_array[ p_l ].name_string                 = obj->item_array[ p_l - 1 ].name_string;
                obj->item_array[ p_l ].time_string                 = obj->item_array[ p_l - 1 ].time_string;
                obj->item_array[ p_l ].real_number_track    = obj->item_array[ p_l - 1 ].real_number_track;
            }

            obj->item_array[ 0 ].name_string = b_char_name;
            obj->item_array[ 0 ].time_string = b_char_time;
            obj->item_array[ 0 ].time_sec    = time_sec;

            obj->f_array->get_item_name_and_time( &obj->item_array[ 0 ], obj->selected->real_number_track - 1 );
            obj->item_array[ 0 ].real_number_track = obj->selected->real_number_track - 1;

            update_play_state( obj );
            return M_INPUT_NOT_HANDLED;
        }

        // Go to the ending.
        obj->selected->selected_state = 0;
        array_reset_to_end( obj );
        obj->selected = &obj->item_array[ obj->item_array_len - 1 ];
        obj->selected->selected_state = 1;

        update_play_state( obj );
        return M_INPUT_NOT_HANDLED;

    case M_KEY_OK:
        // Func return play state track.
        // 0 - track play. 1 - error or stop.
        if ( obj->f_array->item_click( obj->selected ) == 0 ) {        // Success.
            obj->play_file_number = obj->selected->real_number_track;
        }
        update_play_state( obj );
        return M_INPUT_NOT_HANDLED;
    }

    return M_INPUT_NOT_HANDLED;
}

static MFocusEnum focus ( MElement* b, MFocusEnum act ) {
    MPlayList* e = b->data;
    return ( act == M_G_FOCUS_PREV || act == M_G_FOCUS_NEXT )
        ? M_G_FOCUS_NOT_NEEDED
        : M_G_FOCUS_OK;
}

#ifdef __cplusplus
}
#endif

#endif
