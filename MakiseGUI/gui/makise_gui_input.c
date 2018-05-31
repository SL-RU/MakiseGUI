#include "makise_gui_input.h"

MInputData _makise_gui_inp_buffer[MAKISE_GUI_INPUT_BUFFER_LEN] = {0};

void makise_gui_input_send(MHost *h, MInputData d)
{
    if(h == 0)
	return;

    MAKISE_MUTEX_REQUEST(h->input.mutex);

    if(h->input.buf_index[h->input.cur_buf]
       >= MAKISE_GUI_INPUT_BUFFER_LEN)
    {
        //if FIFO is overflowen
	MAKISE_MUTEX_RELEASE(h->input.mutex);
	return;
    }
    //printf("cur %d ind %d key %d\n", h->input.cur_buf, h->input.buf_index[h->input.cur_buf], d.key);
    //
    h->input.buffer[h->input.cur_buf]
	[h->input.buf_index[h->input.cur_buf]++] = d;

    MAKISE_MUTEX_RELEASE(h->input.mutex);
}
void makise_gui_input_send_button(MHost *h, MInputKeyEnum key, MInputEventEnum event, uint32_t time)
{
    MInputData d = {0};
    d.event = event;
    d.key = key;
    d.time = time;
    makise_gui_input_send(h, d);
}
#if MAKISE_GUI_INPUT_POINTER_ENABLE == 1
void makise_gui_input_send_cursor(MHost *h, MInputEventEnum event, int32_t x, int32_t y, uint32_t time)
{
    MInputData d = {0};
    d.event = event;
    d.key = M_KEY_CURSOR;
    d.time = time;
    d.cursor.x = x;
    d.cursor.y = y;
    makise_gui_input_send(h, d);
}
#endif
void makise_gui_input_send_char(MHost *h, char c)
{
    MInputData d = {0};
    d.event = M_INPUT_CHARACTER;
    d.c = c;
    d.key = 0;
    d.time = 0;
    makise_gui_input_send(h, d);
}

#if MAKISE_GUI_INPUT_POINTER_ENABLE == 1
uint8_t _makise_gui_input_perform_cursor(MHost *h, MInputData *d)
{
    if(d->key != M_KEY_CURSOR)
	return 1;
    if(d->event == M_INPUT_MOVE)
	return 0;
    if((h->input.cursor_session & 0b01) && //if session begun
       (d->event == M_INPUT_PRESSING) &&   //and button is pressing
       (h->input.cursor_session & 0b10))   //and input allowed
	return 1;//we can just send event to focused element

    if((!h->input.cursor_session & 0b01) && //if session not begun
       (d->event == M_INPUT_PRESSING))
    {
	MElement *e = makise_g_cont_element_on_point(&h->host,
						     d->cursor.x, d->cursor.y);
	if(e != 0) //if element found
	{
	    //focus element under the cursor
	    if(makise_g_focus(e, M_G_FOCUS_GET) == M_G_FOCUS_OK)
	    {
		//if element under cursor recieved focus
		h->input.cursor_session = 0b11; //start session & allow input
		d->event = M_INPUT_FIRST_PRESS;
		return 1; //we can just send event to focused element
	    }
	    else
	    {
		//if element didn't recieve cursor
		h->input.cursor_session = 0b01; //start session & disable input
		return 0;		
	    }
	}
	else //if no element under the cursor
	{
	    h->input.cursor_session = 0b01; //start session & disable input from cursor
	    return 0;
	}
    }
    if(d->event == M_INPUT_CLICK) //if cursor's button was released
    {
	if((!h->input.cursor_session & 0b01)) //if session not begun
	{
	    MElement *e = makise_g_cont_element_on_point(&h->host,
							 d->cursor.x, d->cursor.y);
	    if(e != 0) //if element found
	    {
		makise_g_focus(e, M_G_FOCUS_GET); //focus element under the cursor;
		h->input.cursor_session = 0; //stop session & allow input
		return 1; //we can just send event to focused element
	    }
	    else
	    {
		h->input.cursor_session = 0; //stop session & allow input
		return 0;
	    }   
	}
	else
	{
	    uint8_t is = h->input.cursor_session >> 1;
	    h->input.cursor_session = 0; //stop session & allow input
	    return is;
	}
    }
    return 0; //if input was forbidden
}
#endif

void makise_gui_input_perform ( MHost *h )
{
    if(h == 0)
	return;
    
    MAKISE_MUTEX_REQUEST(h->input.mutex);
    
    if(h->input.buf_index == 0)
    {
	MAKISE_MUTEX_RELEASE(h->input.mutex);
	return;
    }
    uint8_t k = h->input.cur_buf; //current buffer index
    MInputResultEnum r;
    MInputData d;
    
    h->input.cur_buf = !k; //switch buffers
    h->input.buf_index[h->input.cur_buf] = 0; //reset index

    uint8_t l = 1; //cursor flag
    
    for (uint32_t i = 0; i < h->input.buf_index[k]; i++)
    {
#if MAKISE_GUI_INPUT_POINTER_ENABLE == 1
	l = _makise_gui_input_perform_cursor(h, &h->input.buffer[k][i]);
	//if(l == 0) do nothing
	
#endif
	if(l == 1)
	{
	    r = makise_g_host_input(h, h->input.buffer[k][i]);
	    //printf("k %d r %d\n", h->input.buffer[k][i].key, r);
	    if(h->input.result_handler != 0)
	    {
		d = h->input.result_handler(h->input.buffer[k][i], r);
		if(d.event != M_INPUT_NONE)
		{
		    makise_g_host_input(h, d);
		}
	    }
	}
	l = 1;
    }
    
    MAKISE_MUTEX_RELEASE(h->input.mutex);
}
