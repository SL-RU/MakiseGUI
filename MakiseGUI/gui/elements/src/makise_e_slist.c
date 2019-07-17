#include "makise_e_slist.h"

#if ( MAKISE_E_SLIST > 0 )


static MResult draw   (MElement* b, MakiseGUI *gui  );
static MFocusEnum focus(MElement* b,  MFocusEnum act);
static MInputResultEnum input  (MElement* b, MInputData data);

static char *name = "SList";

void m_create_slist( MSList*                      b,
                     MContainer*                  c,
                     MPosition                    pos,
                     char*                        text,
                     void                         ( *onselection )    ( MSList *l, MSList_Item *selected ),
                     void                         ( *click )          ( MSList *l, MSList_Item *selected ),
                     MSList_Type                  type,
                     const MakiseStyle_SList*     style,
                     const MakiseStyle_SListItem* item_style)
{
    //clear structure
    memset(b, 0, sizeof(MSList));

    MElement *e = &b->el;
    m_element_create(e, name, b,
		     1, MFocusPrior_Focusble,
		     pos,
		     &draw,
		     0,
		     0,
		     &input,
		     &focus,
		     0, 0);
    
    b->text                 = text;

    b->onselection          = onselection;
    b->click                = click;

    b->items                = 0;
    b->is_array             = 0;
    b->len                  = 0;
    b->selected             = 0;

    b->type                 = type;

#if ( MAKISE_GUI_INPUT_POINTER_ENABLE == 1 )
    b->started              = 0;
#endif    

    
    b->style                = style;
    b->item_style           = item_style;
    
    makise_g_cont_add(c, e);

    MAKISE_DEBUG_OUTPUT( "SList %d created\n", e->id );
}

static void setup_text_scroll(MSList *l)
{
    //text scroll setup
    l->text_scroll_x = 0;
    l->text_scroll_width = UINT32_MAX;
}

//draw line frome the list
static void draw_item ( MSList_Item *ci, MSList *l, MakiseGUI *gui,
                        const MakiseStyleTheme *c_th,
			uint32_t x, uint32_t y,
			uint32_t w, uint32_t eh ) {

    makise_d_rect_filled( gui->buffer,
			  (MPoint){x, y}, w, eh,
                          c_th->thickness,
			  c_th->border_c, c_th->bg_color);
    
    switch ( l->type ) {
    case MSList_Checkbox:
	makise_d_rect(gui->buffer,
		      (MPoint){x, y}, eh, eh,
                      c_th->thickness,
		      c_th->font_col);

	if ( ci->value ) {
	    uint32_t d = eh > 21 ? 5 : 2;
	    makise_d_rect_filled(gui->buffer,
				 (MPoint){x + d, y + d},
                                 eh - d * 2, eh - d * 2,
                                 c_th->thickness,
				 c_th->font_col, l->item_style->active.font_col);
	}
	x += eh;
	w -= eh;
	break;

    case MSList_RadioButton:
	if ( eh < 21 ) {
	    makise_d_circle( gui->buffer,
			     (MPoint){x + eh / 2, y + eh / 2}, eh / 2 - 1,
                             c_th->thickness,
			     c_th->font_col  );
	    if ( ci->value )
		makise_d_circle_filled( gui->buffer,
					(MPoint){x + eh / 2, y + eh / 2},
                                        eh / 2 - 3,
                                        c_th->thickness,
					c_th->font_col,
					l->item_style->active.font_col);
	    x += eh + 1;
	    w -= eh + 1;
	} else {
	    makise_d_circle( gui->buffer,
			     (MPoint){x + 10, y + eh/2}, 10,
                             c_th->thickness,
			     c_th->font_col );
	    if(ci->value)
		makise_d_circle_filled( gui->buffer,
					(MPoint){x + 10, y + eh/2}, 7,
                                        c_th->thickness,
					c_th->font_col,
					l->item_style->active.font_col);
	    x += 22;
	    w -= eh + 1;
	}
	break;
    default: break;
    }

    int32_t dx = 0, scrlx = l->text_scroll_x / 100;
    if(l->item_style->text_scroll_speed &&
       l->text_scroll_width > w - 2 &&
       l->selected == ci)
    {
	if(scrlx >= (int32_t)l->text_scroll_width) {
	    l->text_scroll_x = 0;
	    dx = 0;
	}
	else
	{
	    if(scrlx < (int32_t)w / 2)
		dx = 0;
	    else if(scrlx >= (int32_t)(l->text_scroll_width - (w / 4)))
		dx = -(l->text_scroll_width - (w / 4)) + w / 2;
	    else
		dx = -scrlx + w / 2;
	}
	l->text_scroll_x += l->item_style->text_scroll_speed;
	//printf("scrl %d %d %d\n", dx, l->text_scroll_x, l->text_scroll_width);
    }

    MakiseBufferBorderData border =
	makise_add_border(gui->buffer,
			  (MakiseBufferBorder){
			      x, y, w, eh, 0, 0});

    makise_d_string(gui->buffer, ci->text, MDTextAll,
		    x + 2 + dx,
		    y + 2,
		    w - 4,
		    l->item_style->font,
		    c_th->font_col);
    
    makise_rem_border(gui->buffer, border);
}

static MResult draw ( MElement* b, MakiseGUI *gui ) {
    MSList *l = (MSList*)b->data;
    const MakiseStyleTheme *th    = l->state ? &l->style->focused : &l->style->normal;
    const MakiseStyleTheme *i_foc = l->state ? &l->item_style->focused : &l->item_style->active;
    const MakiseStyleTheme *i_nom = &l->item_style->normal,
	*c_th = 0;
    
    //printf("%d %d %d %d\n", b->position.real_x, b->position.real_y, b->position.width, b->position.height);
    _m_e_helper_draw_box_param( gui->buffer, &b->position,
				th->border_c, th->bg_color, th->thickness );

    if(l->text_scroll_width == UINT32_MAX)
        l->text_scroll_width = makise_d_string_get_width(
            gui->buffer,
            l->selected->text, MDTextAll,
            l->item_style->font);

    
    uint32_t i = 0, start = 0, end = 0;
    int16_t y = b->position.real_y,
	x = b->position.real_x + l->style->left_margin;
    uint32_t
	w = b->position.width - l->style->scroll_width,
	h = b->position.height,
	eh = l->item_style->font->height + l->item_style->font_line_spacing + 2 + 2,
	sh = 0,   //scroll line height
	cuid = 0, //current id
	len = 0;  //count of items

    if( l->text != 0 ) {
	makise_d_string_frame( gui->buffer,
			       l->text, MDTextAll,
			       x + 2, y + 2,
			       w - 4, eh,
			       l->item_style->font,
			       l->item_style->font_line_spacing,
			       th->font_col );

	y += l->style->font->height + 4;
	h -= l->style->font->height + 4;    // 2 pixel line (up and down) + 2 space (up and down).

	makise_d_line( gui->buffer,
		       (MPoint){b->position.real_x, y},
		       (MPoint){b->position.real_x + b->position.width},
		       y,
		       th->border_c);
    }

    uint32_t ec = h / (eh - 1); //count of elements on the screen
    // One line total
    y += l->style->item_margin;
    
    MSList_Item *ci = 0;

    if ( ec == 0 ) return M_ERROR;
    
    len = l->len;

    if ( l->is_array ) {
	i = l->selected->id;
    } else {
	//count index in stack of selected item.
	ci = l->items;
	uint32_t ti = 0;
	i = UINT32_MAX;
	while ( ci != 0 ) {
	    if ( ci == l->selected ) {
		cuid = i = ti;
		break;
	    }
	    ci = ci->next;
	    ti ++;
	}
	if ( i == UINT32_MAX ) return M_ERROR;          // if we didn't found it
    }

    //compute start index of element to display & the last
    if ( ec >= len ) {
	start = 0;
	end = len;
    } else if ( (i >= (ec / 2)) && ((len - i) > (ec - 1) / 2) ) {
	start = i - (ec / 2);
	end = start + ec;
    } else if ( ( i > (ec / 2) && (len - i) <= (ec - 1) / 2 ) ) {
	end = len;
	start = len - ec;
    } else if (i < (ec / 2) && (len - i) > (ec - 1) / 2) {
	start = 0;
	end = ec;
    }

    if ( l->is_array ) {
	//array
	for ( i = start; i < end; i++ ) {
	    ci = &l->items[i];
	    ci->id = i;
	    c_th = (ci == l->selected) ? i_foc : i_nom;
	    if ( ci == l->selected ) cuid = i;

	    draw_item( ci, l, gui,
		       c_th,
		       x, y, w, eh );
	    
	    y += eh - 1;
	}
    } else {
	//Linked list
	ci = l->selected;
	while ( i != start ) {
	    i --;
	    ci = ci->prev;
	}

	for ( i = start; i < end; i++ ) {
	    c_th = (ci == l->selected) ? i_foc : i_nom;

	    draw_item( ci, l, gui,
		       c_th,
		       x, y, w, eh );

	    y += eh - 1;
	    ci = ci->next;
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
    if ( l->style->scroll_width != 0 ) {
	makise_d_rect_filled( gui->buffer,
			      (MPoint){ b->position.real_x
                                      + b->position.width
                                      - l->style->scroll_width - 1,
                                      b->position.real_y },
			      l->style->scroll_width + 1,
			      l->el.position.height,
                              th->thickness,
			      th->border_c,
			      l->style->scroll_bg_color );

	makise_d_rect_filled( gui->buffer,
			      (MPoint){ b->position.real_x
                                      + b->position.width
                                      - l->style->scroll_width - 1,
                                      y },
			      l->style->scroll_width + 1,
			      sh + 1,
                              th->thickness,
			      th->border_c,
			      l->style->scroll_color );
    }

    return M_OK;
}

static MFocusEnum focus ( MElement* b, MFocusEnum act ) {
    MSList* e = b->data;
    if ( act & M_G_FOCUS_GET )     e->state = 1;
    if ( act == M_G_FOCUS_LEAVE )  e->state = 0;

    return ( act & M_G_FOCUS_GET ) || ( act & M_G_FOCUS_LEAVE )
	? M_G_FOCUS_OK
	: M_G_FOCUS_NOT_NEEDED;
}

static void input_item ( MSList *e, MSList_Item *it ) {
    switch ( e->type ) {
    case MSList_List:                                       break;
    case MSList_Checkbox:       it->value = !it->value;     break;
    case MSList_RadioButton:
	if ( e->is_array ) {
	    for ( uint32_t i = 0; i < e->len; i++ ) {
		e->items[i].value = 0;
	    }
	} else {
	    MSList_Item *i = e->items;
	    while (i != 0) {
		i->value = 0;
		i = i->next;
	    }
	}
	it->value = 1;
    }
    if ( e->click != 0 ) e->click( e, it );
}

#if ( MAKISE_GUI_INPUT_POINTER_ENABLE == 1 )
static uint8_t input_check_item ( MSList_Item *ci, MSList *l, uint32_t x, uint32_t y, uint32_t w, uint32_t eh, int32_t cx, int32_t cy ) {
    //printf("check %s\n", ci->text);
    if ( cx >= x && cx <= x + w &&
	 cy >= y && cy <= y + eh ) {
	//printf("ok");
	l->selected = ci;
	input_item(l, ci);
	return 1;
    }
    return 0;
}

static MInputResultEnum input_cursor_click ( MSList* l, MElement *b, int32_t cx, int32_t cy ) {
    if ( !( l->sx + 5 > cx && l->sx - 5 < cx &&
	    l->sy + 5 > cy && l->sy - 5 < cy ) ) // if cursor didnt moved a lot.
	return M_INPUT_NOT_HANDLED;

    uint32_t    i       = 0, start = 0, end = 0;
    int16_t     y       = b->position.real_y + 1,
	x       = b->position.real_x + 2;
    uint32_t    w       = b->position.width - 5,
	h       = b->position.height - 4,
	eh      = l->item_style->font->height + l->item_style->font_line_spacing + 3,
	ec      = h / (eh + 1), //count of elements on the screen
	sh      = 0,            //scroll line height
	cuid    = 0,            //current id
	len     = 0;            //count of items

    if ( l->text != 0 ) {
	y += l->style->font->height;
	h -= l->style->font->height;
	ec = h / (eh + 1);
    }
    y+=1;

    MSList_Item *ci = 0;

    if ( ec == 0 ) return M_INPUT_ERROR;
    
    len = l->len;

    if ( l->is_array ) {
	i = l->selected->id;
    } else {
	//count index in stack of selected item.
	ci = l->items;
	uint32_t ti = 0;
	i = UINT32_MAX;
	while ( ci != 0 ) {
	    if(ci == l->selected) {
		cuid = i = ti;
		break;
	    }
	    ci = ci->next;
	    ti ++;
	}
	if ( i == UINT32_MAX )  return M_INPUT_ERROR; //if we didn't found it
    }
    //compute start index of element to display & the last
    if ( ec >= len ) {
	start = 0;
	end = len;
    } else if ( (i >= (ec / 2)) && ((len - i) > (ec - 1) / 2) ) {
	start = i - (ec / 2);
	end = start + ec;
    } else if ( (i > (ec / 2) && (len - i) <= (ec - 1) / 2) ) {
	end = len;
	start = len - ec;
    } else if ( i < (ec / 2) && (len - i) > (ec - 1) / 2 ) {
	start = 0;
	end = ec;
    }
    
    if( l->is_array ) {
	//array
	for ( i = start; i < end; i++ ) {
	    ci = &l->items[i];
	    ci->id = i;

	    if ( ci == l->selected)   cuid = i;

	    if ( input_check_item( ci, l, x, y, w, eh, cx, cy ) )
		return M_INPUT_HANDLED;

	    y += eh + 1;
	}
    } else {
	//Linked list
	ci = l->selected;
	while (i != start) {
	    i --;
	    ci = ci->prev;
	}
	for (i = start; i < end; i++) {
	    if ( input_check_item( ci, l, x, y, w, eh, cx, cy ) )
		return M_INPUT_HANDLED;
	    y += eh + 1;
	    ci = ci->next;
	}
    }
    h = b->position.height - 2;
    sh = h / len;
    if ( sh < 5 ) {
	y = cuid * (h + sh - 5) / len;
	sh = 5;
    } else {
	y = cuid * (h) / len;
    }
    y += b->position.real_y + 1;
    
    return M_INPUT_NOT_HANDLED;
}

static MInputResultEnum input_cursor ( MElement* b, MInputData data ) {
    if(data.key != M_KEY_CURSOR)
	return M_INPUT_NOT_HANDLED;

    MSList *l = b->data;

    switch ( data.event ) {
    case M_INPUT_PRESSING:                                          // If mouse button is pressing.
	if ( l->started ) {                                         // If press is already started
	    //int32_t dx = data.cursor.x - l->sx;
	    int32_t dy = data.cursor.y - l->sy;
	    int32_t eh = l->item_style->font->height +
		l->item_style->font_line_spacing + 3;                   // height of one item

	    float cf = (float)dy / (float)eh;                       // how many items need to scroll
	    if ( eh * l->len > b->position.height * 1.5f )          // if there are many items - we'll speedup scrolling
		cf *= 1.8f;
	    int32_t c = cf;

	    if(l->is_array) {                                       //if array then just compute new index
		MSList_Item *it = l->selected;
		if ( c < 0 ) {
		    if ( l->sitem->id >= -c )
			l->selected = &l->items[l->sitem->id + c];
		    else
			l->selected = l->items;
		} else {
		    if( l->sitem->id + c < l->len )
			l->selected = &l->items[l->sitem->id + c];
		    else
			l->selected = &l->items[l->len - 1];
		}

		if ( it != l->selected ) {
		    //send selected event before

		    setup_text_scroll(l);
		    if ( l->onselection != 0 )
			l->onselection(l, l->selected);
		}
	    } else {
		int32_t i = c < 0 ? -c : c;
		MSList_Item *it = l->sitem;
		while ( i > 0 ) {
		    if ( c < 0 ) {
			if(it->prev == 0) break;
			it = it->prev;
		    } else {
			if ( it->next == 0 ) break;
			it = it->next;
		    }
		    i --;
		}

		if( l->selected != it ) {
		    l->selected = it;
		    
		    setup_text_scroll(l);
		    //send selected event before
		    if ( l->onselection != 0 )
			l->onselection(l, l->selected);
		}
	    }
	} else {                                                         //first touch
	    l->sx = data.cursor.x;
	    l->sy = data.cursor.y;
	    l->sitem = l->selected;
	    l->started = 1;
	}
	return M_INPUT_HANDLED;

    case M_INPUT_CLICK:
	l->started = 0;
	if ( input_cursor_click( b->data, b, data.cursor.x, data.cursor.y ) )
	    return M_INPUT_HANDLED;
    default: break;
    }

    return M_INPUT_NOT_HANDLED;
}

#endif

static MInputResultEnum input ( MElement* b, MInputData data ) {
    MSList *e = ((MSList*)b->data);

    if ( e->items == 0 )
	return M_INPUT_NOT_HANDLED;

    MSList_Item* last_item  = e->selected;
    uint8_t      handled    = 0; //was event handled
    
    if ( e->selected == 0)
	e->selected = e->items;

#if MAKISE_GUI_INPUT_POINTER_ENABLE == 1
    if ( input_cursor  (b,  data) == M_INPUT_HANDLED)
	return M_INPUT_HANDLED;
#endif
    
    if ( data.event == M_INPUT_CLICK ) {
	switch ( data.key ) {
	case M_KEY_DOWN:
	    if ( e->is_array ) {
		MSList_Item *n = 0;
		for ( uint32_t i = 0; i < e->len; i++ ) {
		    e->items[i].id = i;
		    if ( i == ( e->selected->id + 1 ) )
			n = &e->items[i];
		}

		if ( n != 0)
		    e->selected = n;
		else
		    e->selected = e->items;
	    } else {
		if(e->selected->next != 0)
		    e->selected = e->selected->next;
		else
		    e->selected = e->items;
	    }
	    handled = 1;
	    break;

	case M_KEY_UP:
	    if ( e->is_array ) {
		MSList_Item *n = 0;
		for ( uint32_t i = 0; i < e->len; i++ ) {
		    e->items[i].id = i;
		    if ( i == ( e->selected->id - 1 ) )
			n = &e->items[i];
		}
		if ( n != 0 )
		    e->selected = n;
		else
		    e->selected = &e->items[e->len - 1];
	    } else {
		if(e->selected->prev != 0) {
		    e->selected = e->selected->prev;
		} else {
		    MSList_Item *n = e->items;
		    while ( n->next != 0 )
			n = n->next;
		    e->selected = n;
		}
	    }
	    handled = 1;
	    break;
	default: break;
	}

	setup_text_scroll(e);
	// Send selected event before
	if ( e->onselection != 0 && last_item != e->selected )
	    e->onselection(e, e->selected);

	if ( data.key == M_KEY_OK ) {
	    if(e->selected != 0)
		input_item(e, e->selected);
	    handled = 1;
	}
    }
    
    return handled ? M_INPUT_HANDLED : M_INPUT_NOT_HANDLED;
}

void m_slist_add( MSList *l, MSList_Item *item ) {
    if ( l == 0 || item == 0)
	return;
    M_E_MUTEX_REQUEST(l);
    if ( l->is_array ) {
	M_E_MUTEX_RELEASE(l);
	return;
    }
    if ( l->items == 0 )  {                             //add first item
	item->prev      = 0;
	item->next      = 0;
	l->items        = item;
	l->len          = 1;
	l->selected     = item;
	M_E_MUTEX_RELEASE(l);
	return;
    }
    MSList_Item *it = l->items;

    while ( it->next )
	it = it->next;

    it->next    = item;
    item->next  = 0;
    item->prev  = it;
    l->len ++;
    M_E_MUTEX_RELEASE(l);
}

void m_slist_clear ( MSList *l ){
    if ( l == 0)
	return;
    M_E_MUTEX_REQUEST(l);
    l->selected = 0;
    l->is_array = 0;
    l->items = 0;
    M_E_MUTEX_RELEASE(l);
}

void m_slist_remove ( MSList *l, MSList_Item *item ) {
    if ( l == 0 || item == 0)
	return;
    M_E_MUTEX_REQUEST(l);
    
    if ( l->items == 0 )
    {
	M_E_MUTEX_RELEASE(l);
	return;
    }

    if ( l->items == item ) { //if first element
	if ( item->next == 0 ) {
	    l->items = 0;
	    l->len = 0;
	    l->selected = 0;
	    M_E_MUTEX_RELEASE(l);
	    return;
	}
	l->items        = l->items->next;
	l->items->prev  = 0;
	item->next      = 0;
	item->prev      = 0;
	l->selected     = l->items;
	l->len--;
	M_E_MUTEX_RELEASE(l);
	return;
    }

    if( item->next == 0 ) {           //if last element
	if( item->prev == 0 )
	{
	    M_E_MUTEX_RELEASE(l);
	    return; //WTF
	}
	l->len --;
	if(item == l->selected)
	    l->selected = item->prev;
	item->prev->next = 0;
	item->prev = 0;
	M_E_MUTEX_RELEASE(l);
	return;
    }

    if ( item->prev == 0 )
    {
	M_E_MUTEX_RELEASE(l);
	return;  //WTF???
    }

    if( item == l->selected )
	l->selected = item->prev;

    item->next->prev = item->prev;
    item->prev->next = item->next;
    item->prev = 0;
    item->next = 0;
    l->len --;
    M_E_MUTEX_RELEASE(l);
}

void m_slist_set_array ( MSList *l, MSList_Item *array, uint32_t len ) {
    if ( l == 0 || array == 0)
	return;
    M_E_MUTEX_REQUEST(l);
    l->items = array;
    l->len = len;
    l->selected = array;
    l->is_array = 1;
    MSList_Item *lst = 0;

    for ( uint32_t i = 0; i < len; i++ ) {
	array[i].prev = lst;
	array[i].next = ((i + 1) < len) ? &array[i+1] : 0;

	array[i].id = i;

	//printf("%s %d\n", array[i].text, array[i].value);

	lst = &array[i];
    }
    M_E_MUTEX_RELEASE(l);
}

void m_slist_set_list ( MSList *l, MSList_Item *first ) {
    if ( l == 0 )
	return;
    M_E_MUTEX_REQUEST(l);
    l->items = first;
    l->selected = first;
    l->is_array = 0;
    l->len = (first != 0);
    if(first == 0)
    {
	M_E_MUTEX_RELEASE(l);
	return;
    }

    MSList_Item *lst = first;
    while ( lst->next != 0 ) {
	l->len ++;
	lst = lst->next;
    }
    M_E_MUTEX_RELEASE(l);
}

void m_slist_set_header ( MSList *l, const char* text ) {
    if ( l == 0 )
	return;
    M_E_MUTEX_REQUEST(l);
    l->text = text;
    M_E_MUTEX_RELEASE(l);
}


#endif

