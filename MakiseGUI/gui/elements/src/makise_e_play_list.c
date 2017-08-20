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
    obj->item_list      = array;
    obj->len            = len;
    obj->selected       = array;
    obj->is_array       = 1;

    MPlayList_Item *lst = NULL;

    for ( uint32_t i = 0; i < len; i++ ) {
        array[i].prev = lst;
        array[i].next = ( ( i + 1 ) < len ) ? &array[ i + 1 ] : NULL;
        array[i].id = i;
        lst = &array[i];
    }
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
//draw line frome the list
static void draw_item ( MPlayList_Item *pl_i, MPlayList *obj, MakiseStyleTheme_PlayList *c_th, uint32_t x, uint32_t y, uint32_t w, uint32_t eh ) {
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

//**********************************************************************
// Private functions ( system ).
//**********************************************************************
static uint8_t draw ( MElement* b ) {
    MPlayList *obj = ( MPlayList* )b->data;
    MakiseStyleTheme_PlayList *th    = obj->state ? &obj->style->focused : &obj->style->normal;
    MakiseStyleTheme_PlayList *i_foc = obj->state ? &obj->item_style->focused : &obj->item_style->active;
    MakiseStyleTheme_PlayList *i_nom = &obj->item_style->normal,

    *c_th = 0;

    //printf("%d %d %d %d\n", b->position.real_x, b->position.real_y, b->position.width, b->position.height);
    _m_e_helper_draw_box_param( b->gui->buffer, &b->position, th->border_c, th->bg_color, th->double_border );

    uint32_t    i       = 0;
    uint32_t    start   = 0;
    uint32_t    end     = 0;

    int16_t     y       = b->position.real_y;
    int16_t     x       = b->position.real_x  + obj->style->left_margin;
    uint32_t    w       = b->position.width   - obj->style->scroll_width;
    uint32_t    h       = b->position.height;

    uint32_t    eh      = obj->item_style->font->height + obj->item_style->font_line_spacing +
                          2 +        // 2 line board.
                          2;         // 2 space line.

    uint32_t    sh      = 0;         // Scroll line height
    uint32_t    cuid    = 0;         // Current id.
    uint32_t    len     = 0;         //count of items

    MPlayList_Item* ci  = NULL;

    if ( obj->header_text != NULL ) {
        makise_d_string_frame( obj->e.gui->buffer,
                               obj->header_text, MDTextAll,
                               x + 2, y + 2,
                               w - 4, eh,
                               obj->item_style->font,
                               obj->item_style->font_line_spacing,
                               th->font_col );

        y += obj->style->font->height + 4;
        h -= obj->style->font->height + 4;    // 2 pixel line (up and down) + 2 space (up and down).

        makise_d_line( b->gui->buffer,
                       b->position.real_x, y,
                       b->position.real_x + b->position.width,
                       y,
                       th->border_c);
    }

    uint32_t ec = h / (eh - 1);                 // Count of elements on the screen.
                                                // One line total.
    if ( ec == 0 ) return M_ERROR;

    MPlayList_Item* pl_i = 0;

    // Get selected item number.
    if ( obj->is_array ) {
        i = obj->selected->id;
    } else {
        ci = obj->item_list;
        uint32_t ti = 0;
        i = UINT32_MAX;
        while ( ci != 0 ) {
            if ( ci == obj->selected ) {
                cuid = i = ti;
                break;
            }
            ci = ci->next;
            ti ++;
        }
        if ( i == UINT32_MAX ) return M_ERROR;          // if we didn't found it
    }

    len = obj->len;
    //compute start index of element to display & the last
    if ( ec >= len ) {
        start = 0;
        end = len;
    } else if ( ( i >= ( ec / 2 ) ) && ( ( len - i ) > ( ec - 1 ) / 2) ) {
        start = i - (ec / 2);
        end = start + ec;
    } else if ( ( i > (ec / 2) && (len - i) <= ( ec - 1 ) / 2 ) ) {
        end = len;
        start = len - ec;
    } else if ( i < ( ec / 2 ) && ( len - i ) > ( ec - 1 ) / 2) {
        start = 0;
        end = ec;
    }

    if ( obj->is_array ) {
        //array
        for ( i = start; i < end; i++ ) {
            pl_i = &obj->item_list[i];
            pl_i->id = i;
            c_th = (pl_i == obj->selected) ? i_foc : i_nom;
            if ( pl_i == obj->selected ) cuid = i;

            draw_item( pl_i, obj, c_th, x, y, w, eh );

            y += eh - 1;
        }
    } else {
        //Linked list
        pl_i = obj->selected;
        while ( i != start ) {
            i --;
            pl_i = pl_i->prev;
        }

        for ( i = start; i < end; i++ ) {
            c_th = (pl_i == obj->selected) ? i_foc : i_nom;

            draw_item( pl_i, obj, c_th, x, y, w, eh );

            y += eh - 1;
            pl_i = pl_i->next;
        }
    }

    h = b->position.height - 2;
    sh = h / len;
    if(sh < 5)
    {
        y = cuid * (h + sh - 5) / len;
        sh = 5;
    }
    else
        y = cuid * (h) / len;
    y += b->position.real_y + 1;


    // Drawing scroll.
    if ( obj->style->scroll_width != 0 ) {
        makise_d_rect_filled( b->gui->buffer,
                  b->position.real_x + b->position.width - obj->style->scroll_width - 1, b->position.real_y,
                  obj->style->scroll_width + 1,
                  obj->e.position.height,
                  th->border_c,
                  obj->style->scroll_bg_color );

        makise_d_rect_filled( b->gui->buffer,
                  b->position.real_x + b->position.width - obj->style->scroll_width - 1,
                  y,               // BUG!
                  obj->style->scroll_width + 1,
                  sh + 1,
                  th->border_c,
                  obj->style->scroll_color );
    }

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
