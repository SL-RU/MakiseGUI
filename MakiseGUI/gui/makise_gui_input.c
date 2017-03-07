#include "makise_gui_input.h"

MInputData _makise_gui_inp_buffer[MAKISE_GUI_INPUT_BUFFER_LEN] = {0};

void makise_gui_input_send(MHost *h, MInputData d)
{
    if(h == 0 ||
       h->input.buf_index[h->input.cur_buf] >= MAKISE_GUI_INPUT_BUFFER_LEN) //if FIFO is overflowen
	return;
    printf("cur %d ind %d key %d\n", h->input.cur_buf, h->input.buf_index[h->input.cur_buf], d.key);
    h->input.buffer[h->input.cur_buf][
	h->input.buf_index[h->input.cur_buf]++
	] = d;
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

#endif

void makise_gui_input_perform(MHost *h)
{
    if(h == 0 ||
       h->input.buf_index == 0)
	return;
    
    uint8_t k = h->input.cur_buf; //current buffer index
    MInputResultEnum r;
    MInputData d;
    
    h->input.cur_buf = !k; //switch buffers
    h->input.buf_index[h->input.cur_buf] = 0; //reset index

    
    for (uint32_t i = 0; i < h->input.buf_index[k]; i++)
    {
#if MAKISE_GUI_INPUT_POINTER_ENABLE == 1
#endif
	r = makise_g_host_input(h, h->input.buffer[k][i]);
	if(h->input.result_handler != 0)
	{
	    d = h->input.result_handler(h->input.buffer[k][i], r);
	    if(d.event != M_INPUT_NONE)
	    {
		makise_g_host_input(h, d);
	    }
	}
    }
    
}
