#include "makise_e_fsviewer.h"

#if MAKISE_E_FSVIEWER > 0

static uint8_t draw   (MElement* b, MakiseGUI *gui  );
static MFocusEnum focus(MElement* b,  MFocusEnum act);
static MInputResultEnum input  (MElement* b, MInputData data);
void m_fsviewer_loadchunk(
    MFSViewer *l,
    uint32_t required_id); //load chunk with required position

static char *name = "MFSViewer";

void m_create_fsviewer( MFSViewer*                  b,
                        MContainer*                 c,
                        MPosition                   pos,
                        MFSViewer_Type              type,
                        MakiseStyle_FSViewer*       style,
                        MakiseStyle_FSViewer_Item*  item_style )
{
    //clear structure
    memset(b, 0, sizeof(MFSViewer));

    
    MElement *e = &b->el;
    m_element_create(e, name, b,
		     1, MFocusPrior_Focusble, pos,
		     &draw,
		     0,
		     0,
		     &input,
		     &focus,
		     0, 0);
    
    b->header = 0;
    
    b->onselection = 0;
    b->click = 0;

    b->path = 0;
    
    b->selected_file[0] = 0;
    b->current_position = 0;
    b->files_count = 0;

    b->type = type;

    b->was_selected = 0;
    b->current_folder = 0;
    
    b->style = style;
    b->item_style = item_style;
    
    makise_g_cont_add(c, e);

    MAKISE_DEBUG_OUTPUT("MFSViewer %d created\n", e->id);
}

//draw single item of the list
static void draw_item   (MFSViewer_Item *ci, MFSViewer *l, MakiseGUI *gui,
			 MakiseStyleTheme_FSViewer_Item *c_th, uint8_t selected,
			 uint32_t x, uint32_t y, uint32_t w, uint32_t eh)
{
    
    //background
    makise_d_rect_filled(gui->buffer,
			 (MPoint){x, y},  w, eh,
                         c_th->thickness,
			 c_th->border_c, c_th->bg_color);
    //selection
    if(l->type == MFSViewer_SingleSelect)
    {
	if(l->was_selected && ci->id == l->selected_file_id
	   && l->selected_folder == l->current_folder)
	{
	    makise_d_circle_filled(gui->buffer,
				   (MPoint){x + eh / 2, y + eh / 2}, eh / 2 - 3,
                                   c_th->thickness,
				   c_th->icon_col, c_th->icon_col);
	    x += eh + 1;
	    w -= eh + 1;
	}
    }
    //directory
    if(ci->am_dir)
    {
	if(l->style->bitmap_folder == 0)
	{
	    makise_d_rect_filled(gui->buffer,
				 (MPoint){x + 2, y + 2}, eh - 4, eh - 4,
                                 c_th->thickness,
				 c_th->icon_col, c_th->icon_col);
	    x += eh + 1;
	    w -= eh + 1;
	}
	else //icon
	{
	    int yc = l->style->bitmap_folder->height / 2; //place icon in center
	    yc = (eh / 2) - yc; 
	    makise_d_bitmap(gui->buffer, x + 1, y + yc,
			    l->style->bitmap_folder, c_th->icon_col);
	    x += l->style->bitmap_folder->width + 2;
	    w -= l->style->bitmap_folder->width + 2;
	}
	
    }
    if(ci->text_width == INT16_MAX)
        ci->text_width = makise_d_string_get_width(gui->buffer,
                                                   ci->name, MDTextAll,
                                                   l->item_style->font);

    //text scrolling
    int32_t dx = 0, scrlx = ci->scroll_x / 100;
    if(l->item_style->text_scroll_speed &&
       ci->text_width > w - 2 &&
       selected)
    {
	if(scrlx >= (int32_t)ci->text_width) {
	    ci->scroll_x = 0;
	    dx = 0;
	}
	else
	{
	    if(scrlx < (int32_t)w / 2)
		dx = 0;
	    else if(scrlx >= (int32_t)(ci->text_width - (w / 4)))
		dx = -(ci->text_width - (w / 4)) + w / 2;
	    else
		dx = -scrlx + w / 2;
	}
	ci->scroll_x += l->item_style->text_scroll_speed;
    }
    else
	ci->scroll_x = 0;

    MakiseBufferBorderData border =
	makise_add_border(gui->buffer,
			  (MakiseBufferBorder){
			      x, y, w, eh, 0, 0});
    //text
    makise_d_string(gui->buffer, ci->name, MDTextAll,
		    x + 1 + dx,
		    y,
		    w - 2,
		    l->item_style->font,
		    c_th->font_col);
	    
    makise_rem_border(gui->buffer, border);
}
static uint8_t draw ( MElement* b, MakiseGUI *gui )
{
    MFSViewer *l = (MFSViewer*)b->data;
    MakiseStyleTheme *th = l->state ? &l->style->focused : &l->style->normal;
    MakiseStyleTheme_FSViewer_Item *i_foc =
	l->state ? &l->item_style->focused : &l->item_style->active;
    MakiseStyleTheme_FSViewer_Item *i_nom = &l->item_style->normal,
	*c_th = 0;
    

    //MAKISE_DEBUG_OUTPUT("%d %d %d %d\n", b->position.real_x, b->position.real_y, b->position.width, b->position.height);
    _m_e_helper_draw_box(gui->buffer, &b->position, th);

    uint32_t i = 0, start = 0, end = 0;
    int16_t y = b->position.real_y + 1,
	x = b->position.real_x + 2;
    uint32_t
	w = b->position.width - 5,
	h = b->position.height - 4,
	eh = l->item_style->font->height + l->item_style->font_line_spacing + 1,
	ec = h / (eh + 1),  // count of elements on the screen
	sh      = 0,        // scroll line height
	cuid    = 0,        // current id
	len     = 0;        // count of items


    //header
    if ( l->header != 0 ) {
        makise_d_string( gui->buffer,
                         l->header,
                         MDTextAll,
                         x, y,
                         MDTextPlacement_LeftUp,
                         l->style->font,
                         th->font_col );

        y += l->style->font->height;
        h -= l->style->font->height;

        makise_d_line( gui->buffer,
                       (MPoint){b->position.real_x, y},
                       (MPoint){b->position.real_x + b->position.width, y},
                       th->thickness,
                       th->border_c);

        ec = h / (eh + 1);
    }
    y += 1;
    
    MFSViewer_Item *ci = 0;

    if(ec == 0)
	return M_ERROR;
    
    len = l->files_count;

    l->displayed_count = ec;

    cuid = i = l->current_position;
    
    //compute start index of element to display & the last
    if ( ec >= len ) {
        start   = 0;
        end     = len;
    } else if ( ( i >= ( ec / 2 ) ) && ( ( len - i ) > ( ec - 1 ) / 2 ) ) {
        start   = i - (ec / 2);
        end     = start + ec;
    } else if ( ( i > ( ec / 2 ) && ( len - i ) <= ( ec - 1 ) / 2 ) ) {
        end     = len;
        start   = len - ec;
    } else if ( i < ( ec / 2 ) && ( len - i ) > ( ec - 1 ) / 2) {
        start   = 0;
        end     = ec;
    }
    
    //check if required chunk is loaded
    if ( start < l->current_chunk_position ||
         end >= ( l->current_chunk_position + FM_BUFFERED ) ) {
        //load chunk
        m_fsviewer_loadchunk(l, i);
    }

    //MAKISE_DEBUG_OUTPUT("start %d end %d\n", start, end);
    //array
    for ( i = start; i < end; i++ ) {
        ci = &l->buffer[i - l->current_chunk_position];
        //MAKISE_DEBUG_OUTPUT("draw %d %s\n", i, ci->name);
        ci->id = i;
        c_th = (i == l->current_position) ? i_foc : i_nom;

        draw_item(ci, l, gui,
		  c_th, (i == l->current_position),
		  x, y, w, eh);
        y += eh + 1;
    }
    

    if(len == 0)
	return M_OK;
    
    h = b->position.height - 2;
    sh = h / len;
    if ( sh < 5 ) {
        y = cuid * (h + sh - 5) / len;
        sh = 5;
    } else {
        y = cuid * (h) / len;
    }
    y += b->position.real_y + 1;

    
    // Drawing scroll.
    if ( l->style->scroll_width != 0 ) {
	makise_d_rect_filled( gui->buffer,
			      (MPoint){b->position.real_x
                                      + b->position.width
                                      - l->style->scroll_width
                                      - 1,
                                      b->position.real_y},
			      l->style->scroll_width + 1,
			      l->el.position.height,
                              th->thickness,
			      th->border_c,
			      l->style->scroll_bg_color );

	makise_d_rect_filled( gui->buffer,
			      (MPoint){b->position.real_x
                                      + b->position.width
                                      - l->style->scroll_width - 1,
                                      y},
			      l->style->scroll_width + 1,
			      sh + 1,
                              th->thickness,
			      th->border_c,
			      l->style->scroll_color );
    }
        
    return M_OK;
}

static MFocusEnum focus (MElement* b,  MFocusEnum act)
{
    //MFSViewer *e = ((MFSViewer*)b->data);
    if(act & M_G_FOCUS_GET)    {
        ((MFSViewer*)b->data)->state = 1;
    } if ( act == M_G_FOCUS_LEAVE )    {
        ((MFSViewer*)b->data)->state = 0;
    }

    return (act == M_G_FOCUS_PREV || act == M_G_FOCUS_NEXT)
	? M_G_FOCUS_NOT_NEEDED
	: M_G_FOCUS_OK;

}

static MInputResultEnum input  (MElement* b, MInputData data)
{
    MFSViewer *e = ((MFSViewer*)b->data);

    if(e->files_count == 0)
	return M_INPUT_NOT_HANDLED;

    uint8_t handled = 0; //was event handled
    
    if(e->current_position >= e->files_count)
    {
	e->current_position = 0;
    }

    
    if(data.event == M_INPUT_CLICK)
    {
	if(data.key == M_KEY_DOWN)
	{
	    if(e->current_position < e->files_count - 1)
	    {
		e->current_position ++;
	    }
	    else
		e->current_position = 0;
	    handled = 1;
	} else if(data.key == M_KEY_UP)
	{
	    if(e->current_position > 0)
	    {
		e->current_position --;
	    }
	    else
		e->current_position = e->files_count - 1;
	    handled = 1;
	}
	//send selected event before 
	/* if(e->onselection != 0 && last_item != e->selected) */
	/*     e->onselection(e, e->selected); */

	if(data.key == M_KEY_OK)
	{
	    MFSViewer_Item *it = &e->buffer[e->current_position -
					    e->current_chunk_position];
	    if(it->am_dir)
	    {
		//if directory
		char trackname[13];
		for (int i = 0; i < 13; i++) {
		    trackname[i] = (char)(it->fname[i]);
		}
		_fsviewer_open(e, it->fname);
	    }
	    else
	    {
		e->was_selected = 1;
		e->selected_folder = e->current_folder;
		
		if(strncmp(e->selected_file, it->name,
#if MAKISE_E_FSVIEWER == MAKISE_E_FSVIEWER_FATFS    
			   13 ) != 13
#else //STDIO
		   256 != 0
#endif
		   && e->onselection != 0
		   //call onselection
		   && e->onselection(e, it))
		{		
		    e->selected_file_id = it->id;
		    if(e->click != 0)
		    {
			e->click(e, it);
		    }
		}
	    }
	    handled = 1;
	}
	
    }
    
    return handled ? M_INPUT_HANDLED : M_INPUT_NOT_HANDLED;
}
#endif
