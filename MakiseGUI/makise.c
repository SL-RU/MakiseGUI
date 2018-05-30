#include "makise.h"

uint32_t makise_init(MakiseGUI * gui, MakiseDriver* driver, MakiseBuffer* buffer)
{
    gui->driver = driver;
    gui->buffer = buffer;
    gui->draw = 0;
    
    buffer->height = driver->lcd_height;
    buffer->width = driver->lcd_width;
    //allowed region for new drawings
    buffer->border = (MakiseBufferBorder)
	{ .x = 0,
	  .y = 0,
	  .w = driver->lcd_width,
	  .h = driver->lcd_height,
	  .ex = driver->lcd_width,
	  .ey = driver->lcd_height };
    
    driver->gui = gui;
    buffer->gui = gui;

#ifdef MAKISEGUI_BUFFER_DEPTHMASK
    buffer->depthmask = MAKISEGUI_BUFFER_DEPTHMASK;
    buffer->pixeldepth = MAKISEGUI_BUFFER_DEPTH;
#else
    buffer->depthmask = 0;
    buffer->pixeldepth = 0;
#endif
    buffer->width = driver->lcd_width;
    buffer->height = driver->lcd_height;

    
    //buffer len in bits
    uint32_t len = buffer->height * buffer->width * buffer->pixeldepth;
    //count in bytes
    uint32_t lenb = len/8;
    if(lenb * 8 < len)
	lenb += 1;

    buffer->size = lenb;
    
    return lenb;
}

//if partial_render = 0, then entire buffer will be rendered, if == 1, then will be rendered only first part, if == 2 then will be rendered second part
void makise_render(MakiseGUI *gui, uint8_t partial_render)
{
    MakiseDriver * d = gui->driver;
    uint16_t c;

    uint32_t y = d->posy, //start position y
	x = 0,
	i = 0, //pixel in drivers's buffer
	cu,
	bu = 0,
	m  = 0; //end position y

    if(partial_render == 0) //render full buffer
    {
	m = d->posy + d->buffer_height;
	d->posy += d->buffer_height;
	//partial_render = 1;
    }
    else if(partial_render == 1) //render first half (called by halfcplt callback)
    {
	m = d->posy + (d->buffer_height / 2);
	y = d->posy;
    }
    else if(partial_render == 2) //render second half (called by cplt callback)
    {
	m = d->posy + d->buffer_height;
	y = d->posy + (d->buffer_height / 2);
	i = d->size / 4;
	d->posy += d->buffer_height;
    }

    cu = (y * gui->buffer->width) * gui->buffer->pixeldepth/32;
    for (; y < m; y++) {	
	for (x = 0; x < d->buffer_width; x+=1)
	{
	    c = makise_color_get(((((uint32_t*)gui->buffer->buffer)[cu]) >> bu) & gui->buffer->depthmask);
	    bu += gui->buffer->pixeldepth;
	    if(bu>=32)
	    {
		bu = 0;
		cu ++;
	    }
	    //c+=bc;
	    ((uint16_t*)d->buffer)[i] = c;
	    i+=1;
	}
    }
    
}

MakiseBufferBorderData makise_add_border(MakiseBuffer *buffer, MakiseBufferBorder b)
{
    if(buffer == 0)
	return (MakiseBufferBorderData){0};

    if(b.x < buffer->border.x)
	b.x = buffer->border.x;
    if(b.y < buffer->border.y)
	b.y = buffer->border.y;
    if(b.w > buffer->border.w + buffer->border.x  - b.x)
	b.w = buffer->border.w + buffer->border.x - b.x;
    if(b.h > buffer->border.h + buffer->border.y - b.y)
	b.h = buffer->border.h + buffer->border.y - b.y;

    b.ex = b.x + b.w;
    b.ey = b.y + b.h;

    MakiseBufferBorder l = buffer->border;
    
    buffer->border = b;
    
    return (MakiseBufferBorderData){b, l};
}
void makise_rem_border(MakiseBuffer *buffer, MakiseBufferBorderData b)
{
    if(buffer == 0)
	return;

    buffer->border = b.last_border;
}
