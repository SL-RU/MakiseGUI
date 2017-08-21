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

// Set new data source. Simple array.
void m_play_list_init_item_array ( MPlayList_Item *array, uint32_t len ) {
    MPlayList_Item *lst = NULL;

    for ( uint32_t i = 0; i < len; i++ ) {
        array[i].stait = 0;
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

    obj_struct->item_array_len          = pos.height / ( obj_struct->eh - 1 );  // Count of elements on the screen.
                                                                                // One line total.

    obj_struct->item_array              = user_func->create_array_item( obj_struct->item_array_len );
    m_play_list_init_item_array( obj_struct->item_array, obj_struct->item_array_len );
    obj_struct->selected                = obj_struct->item_array;
    obj_struct->selected ->stait        = 2;         // Выбранный трек.
    obj_struct->focus_file_number       = 0;

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

    makise_d_string_frame( obj->e.gui->buffer,
                           pl_i->name, MDTextAll,
                           x + 2, y + 2,
                           w - 4, eh,
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

static void drawing_scroll ( const MPlayList* obj, const MakiseStyleTheme_PlayList* th, const int16_t* y_pos, const uint32_t* h_scroll ) {
    if ( obj->style->scroll_width != 0 ) {
        makise_d_rect_filled( obj->e.gui->buffer,
                  obj->e.position.real_x + obj->e.position.width - obj->style->scroll_width, obj->e.position.real_y,
                  obj->style->scroll_width,
                  obj->e.position.height,
                  th->border_c,
                  obj->style->scroll_bg_color );

        makise_d_rect_filled( obj->e.gui->buffer,
                  obj->e.position.real_x + obj->e.position.width - obj->style->scroll_width,
                  *y_pos,
                  obj->style->scroll_width,
                  *h_scroll,
                  th->border_c,
                  obj->style->scroll_color );
    }

}

//**********************************************************************
// Private functions ( system ).
//**********************************************************************
static uint8_t draw ( MElement* b ) {
    MPlayList *obj = ( MPlayList* )b->data;
    /*
    MakiseStyleTheme_PlayList *th    = obj->state ? &obj->style->focused : &obj->style->normal;
    MakiseStyleTheme_PlayList *i_foc = obj->state ? &obj->item_style->focused : &obj->item_style->active;
    MakiseStyleTheme_PlayList *i_nom = &obj->item_style->normal,*/

    MakiseStyleTheme_PlayList* st = &obj->style->theme;
    _m_e_helper_draw_box_param( b->gui->buffer, &b->position, st->border_c, st->bg_color, st->double_border );

    int16_t     y       = b->position.real_y;
    int16_t     x       = b->position.real_x  + obj->style->left_margin;
    uint32_t    w       = b->position.width   - obj->style->scroll_width;
                w      += ( obj->style->scroll_width != 0 ) ? 1 : 0;
    uint32_t    h       = b->position.height;

    header_text_draw( obj, &x, &y, &obj->eh, &h );                   // Draw header.
    uint32_t    scroll_y    = y;

    for ( uint32_t i = 0; i < obj->item_array_len; i++ ) {
        switch ( obj->item_array[ i ].stait ) {
        case 2: st = &obj->item_style->selected; break;
        case 1: st = &obj->item_style->play;     break;
        case 0: st = &obj->item_style->normal;   break;
        }

        draw_item( obj, &obj->item_array[ i ], st, x, y, w, obj->eh );

        y += obj->eh - 1;
    }

    if ( obj->style->scroll_width == 0 ) return M_OK;

  //  drawing_scroll( obj, th, &y, &h );

    return M_OK;
}

static MFocusEnum focus ( MElement* b, MFocusEnum act ) {

}

static MInputResultEnum input ( MElement* b, MInputData data ) {

}

#ifdef __cplusplus
}
#endif

#endif
