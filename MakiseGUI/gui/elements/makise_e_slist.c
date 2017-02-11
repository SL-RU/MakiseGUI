#include "makise_e.h"

uint8_t _m_slist_draw   (MElement* b);
MFocusEnum _m_slist_focus(MElement* b,  MFocusEnum act);
MInputResultEnum _m_slist_input  (MElement* b, MInputData data);

void m_create_slist(MSList* b, MContainer *c,
		    int32_t x, int32_t y, uint32_t w, uint32_t h,
		    char* text,
		    void (*onselection)(MSList *l, MSList_Item *selected),
		    void (*click)(MSList *l, MSList_Item *selected),
		    MSList_Type type,
		    MakiseStyle *style,
		    MakiseStyle *item_style)
{
    MElement *e = &b->el;
    e->gui = c->gui;

    e->data = b;
    
    makise_g_cont_add(c, e);

    e->draw = &_m_slist_draw;
    e->predraw = 0;
    e->update = 0;
    e->input = &_m_slist_input;
    e->focus = &_m_slist_focus;
    e->free =  0;
	
    e->position.x = x;
    e->position.y = y;
    e->position.width = w;
    e->position.height = h;

    e->enabled = 1;
    e->id = makise_g_newid();
    e->focus_prior = 1;
    
    b->text = text;

    b->onselection = onselection;
    b->click = click;

    b->items = 0;
    b->is_array = 0;
    b->len = 0;
    b->selected = 0;

    b->type = type;
    
    b->style = style;
    b->item_style = item_style;
    
    printf("Slist %d created\n", e->id);
}

void _m_slist_draw_item   (MSList_Item *ci, MSList *l, MakiseStyleTheme *c_th, uint32_t x, uint32_t y, uint32_t w, uint32_t eh)
{
    //draw line frome the list
    
    makise_d_rect_filled(l->el.gui->buffer,
			 x, y, w, eh,
			 c_th->border_c, c_th->bg_color);
    
    if(l->type == MSList_Checkbox)
    {
	makise_d_rect(l->el.gui->buffer,
		      x, y, eh, eh,
		      c_th->font_col);

	if(ci->value)
	    makise_d_rect_filled(l->el.gui->buffer,
				 x + 2, y + 2, eh - 4, eh - 4,
				 c_th->font_col, l->item_style->active.font_col);
	x += eh;
	w -= eh;
    } else if(l->type == MSList_RadioButton)
    {
	makise_d_circle(l->el.gui->buffer,
		      x + eh / 2, y + eh / 2, eh / 2 - 1,
		      c_th->font_col);

	if(ci->value)
	    makise_d_circle_filled(l->el.gui->buffer,
				 x + eh / 2, y + eh / 2, eh / 2 - 3,
				 c_th->font_col, l->item_style->active.font_col);
	x += eh + 1;
	w -= eh + 1;
    }
    
    
    makise_d_string_frame(l->el.gui->buffer, ci->text, MDTextAll,
			  x + 1,
			  y,
			  w - 2, eh,
			  l->item_style->font,
			  l->item_style->font_line_spacing,
			  c_th->font_col);
	    
    
}
uint8_t _m_slist_draw   (MElement* b)
{
    MSList *l = (MSList*)b->data;
    MakiseStyleTheme *th = l->state ? &l->style->focused : &l->style->normal;
    MakiseStyleTheme *i_foc =l->state ? &l->item_style->focused : &l->item_style->active;
    MakiseStyleTheme *i_nom = &l->item_style->normal,
	*c_th = 0;
    
    
    _m_e_helper_draw_box(b->gui->buffer, &b->position, th);

    uint32_t i = 0, start = 0, end = 0;
    int16_t y = b->position.real_y + 1,
	x = b->position.real_x + 2;
    uint32_t
	w = b->position.width - 5,
	h = b->position.height - 4,
	eh = l->item_style->font->height + l->item_style->font_line_spacing + 3,
	ec = h / (eh + 1), //count of elements on the screen
	sh = 0,   //scroll line height
	cuid = 0, //current id
	len = 0;  //count of items



    if(l->text != 0)
    {
	makise_d_string(b->gui->buffer,
			l->text, MDTextAll,
			x, y, MDTextPlacement_LeftUp,
			l->style->font, th->font_col);
	y += l->style->font->height;
	h -= l->style->font->height;
	makise_d_line(b->gui->buffer, b->position.x, y,
		      b->position.x + b->position.width, y,
		      th->border_c);
	ec = h / (eh + 1);
    }
    y += 1;
    
    MSList_Item *ci = 0;

    if(ec == 0)
	return M_ERROR;
    
    len = l->len;
	
    if(l->is_array)
    {
	i = l->selected->id;
    }
    else
    {
	//count index in stack of selected item.
	ci = l->items;
	uint32_t ti = 0;
	i = UINT32_MAX;
	while (ci != 0)
	{
	    if(ci == l->selected)
	    {
		cuid = i = ti;
		break;
	    }
	    ci = ci->next;
	    ti ++;
	}
	if(i == UINT32_MAX)
	    //if we didn't found it
	    return M_ERROR;
    }
    //compute start index of element to display & the last
    if(ec >= len)
    {
	start = 0;
	end = len;
    }
    else if ((i >= (ec / 2)) && ((len - i) > (ec - 1) / 2))
    {
	start = i - (ec / 2);
	end = start + ec;
    }
    else if ((i > (ec / 2) && (len - i) <= (ec - 1) / 2))
    {
	end = len;
	start = len - ec;
    }
    else if (i < (ec / 2) && (len - i) > (ec - 1) / 2)
    {
	start = 0;
	end = ec;
    }
    
    if(l->is_array)
    {
	//array
	for (i = start; i < end; i++)
	{
	    ci = &l->items[i];
	    ci->id = i;
	    c_th = (ci == l->selected) ? i_foc : i_nom;
	    if(ci == l->selected)
		cuid = i;

	    _m_slist_draw_item(ci, l, c_th, x, y, w, eh);
	    y += eh + 1;
	}
    }
    else
    {
	//Linked list
	ci = l->selected;
	while (i != start) {
	    i --;
	    ci = ci->prev;
	}
	for (i = start; i < end; i++)
	{
	    c_th = (ci == l->selected) ? i_foc : i_nom;

	    _m_slist_draw_item(ci, l, c_th, x, y, w, eh);

	    y += eh + 1;
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
    
    makise_d_line(b->gui->buffer,
		  x + w - 1, b->position.real_y,
		  x + w - 1, b->position.real_y + b->position.height - 1,
		  th->border_c);
    makise_d_line(b->gui->buffer,
		  x + w    , y,
		  x + w    , y + sh,
		  th->font_col);
    makise_d_line(b->gui->buffer,
		  x + w + 1, y,
		  x + w + 1, y + sh,
		  th->font_col);
    return M_OK;
}

void m_slist_add(MSList *l, MSList_Item *item)
{
    if(l->is_array)
	return;
    if(l->items == 0) //add first item
    {
	item->prev = 0;
	item->next = 0;
	l->items = item;
	l->len = 1;
	l->selected = item;
	return;
    }
    MSList_Item *it = l->items;
    while (it->next) {
	it = it->next;
    }
    it->next = item;
    item->next = 0;
    item->prev = it;
    l->len ++;
}
void m_slist_clear(MSList *l)
{
}
void m_slist_remove(MSList *l, MSList_Item *item)
{
    if(l == 0 || item == 0 || l->items == 0)
	return;
    if(l->items == item) //if first element
    {
	if(item->next == 0)
	{
	    l->items = 0;
	    l->len = 0;
	    l->selected = 0;
	    return;
	}
	l->items = l->items->next;
	l->items->prev = 0;
	item->next = 0;
	item->prev = 0;
	l->selected = l->items;
	l->len --;
	return;
    }
    if(item->next == 0) //if last element
    {
	if(item->prev == 0) //WTF
	    return;
	l->len --;
	if(item == l->selected)
	    l->selected = item->prev;
	item->prev->next = 0;
	item->prev = 0;
	return;
    }
    if(item->prev == 0) //WTF???
	return;
    if(item == l->selected)
	l->selected = item->prev;
    item->next->prev = item->prev;
    item->prev->next = item->next;
    item->prev = 0;
    item->next = 0;
    l->len --;
}
void m_slist_set_array(MSList *l, MSList_Item *array, uint32_t len)
{
    l->items = array;
    l->len = len;
    l->selected = array;
    l->is_array = 1;
    MSList_Item *lst = 0;
    for (uint32_t i = 0; i < len; i++)
    {
	array[i].prev = lst;
	array[i].next = ((i + 1) < len) ? &array[i+1] : 0;

	array[i].id = i;
	
	lst = &array[i];
    }
}
void m_slist_set_list(MSList *l, MSList_Item *first)
{
    l->items = first;
    l->selected = first;
    l->is_array = 0;
    l->len = (first != 0);
    if(first != 0)
    {
	MSList_Item *lst = first;
	while (lst->next != 0)
	{
	    l->len ++;
	    lst = lst->next;
	}
    }
}

MFocusEnum _m_slist_focus   (MElement* b,  MFocusEnum act)
{
    MSList *e = ((MSList*)b->data);
    if(act & M_G_FOCUS_GET)
    {
	((MSList*)b->data)->state = 1;
    }
    if(act == M_G_FOCUS_LEAVE)
    {
	((MSList*)b->data)->state = 0;
    }

    return (act == M_G_FOCUS_PREV || act == M_G_FOCUS_NEXT)
	? M_G_FOCUS_NOT_NEEDED
	: M_G_FOCUS_OK;

}
void _m_slist_input_item  (MSList *e, MSList_Item *it)
{
    if(e->type == MSList_List)
    {
    } else if(e->type == MSList_Checkbox)
    {
	it->value = !it->value;
    } else if(e->type == MSList_RadioButton)
    {
	if(e->is_array)
	{
	    for (uint32_t i = 0; i < e->len; i++) {
		e->items[i].value = 0;
	    }
	}
	else
	{
	    MSList_Item *i = e->items;
	    while (i != 0) {
		i->value = 0;
		i = i->next;
	    }
	}
	it->value = 1;
    }
    if(e->click != 0)
	e->click(e, it);
}
MInputResultEnum _m_slist_input  (MElement* b, MInputData data)
{
    MSList *e = ((MSList*)b->data);

    if(e->items == 0)
	return M_INPUT_NOT_HANDLED;
    if(e->selected == 0)
    {
	e->selected = e->items;
    }
    
    if(data.event == M_INPUT_CLICK)
    {
	if(data.key == M_KEY_DOWN)
	{
	    if(e->is_array)
	    {
		MSList_Item *n = 0;
		for (uint32_t i = 0; i < e->len; i++)
		{
		    e->items[i].id = i;
		    if(i == (e->selected->id + 1))
		    {
			n = &e->items[i];
		    }
		}
		if(n != 0)
		    e->selected = n;
		else
		    e->selected = e->items;
	    } else
	    {
		if(e->selected->next != 0)
		    e->selected = e->selected->next;
		else
		    e->selected = e->items;
	    }
	} else if(data.key == M_KEY_UP)
	{
	    if(e->is_array)
	    {
		MSList_Item *n = 0;
		for (uint32_t i = 0; i < e->len; i++)
		{
		    e->items[i].id = i;
		    if(i == (e->selected->id - 1))
		    {
			n = &e->items[i];
		    }
		}
		if(n != 0)
		    e->selected = n;
		else
		    e->selected = &e->items[e->len - 1];
	    }
	    else
	    {
		if(e->selected->prev != 0)
		    e->selected = e->selected->prev;
		else
		{
		    MSList_Item *n = e->items;
		    while (n->next != 0)
		    {
			n = n->next;
		    }
		    e->selected = n;
		}
	    }
	} else if(data.key == M_KEY_OK)
	{
	    if(e->selected != 0)
		_m_slist_input_item(e, e->selected);
	}
	
	
	e->onselection(e, e->selected);
	return M_INPUT_HANDLED;
    }
    
    return M_INPUT_NOT_HANDLED;

}
