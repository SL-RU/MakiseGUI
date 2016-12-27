#include "makise.h"

uint32_t makise_init(MakiseGUI * gui, MakiseDriver* driver, MakiseBuffer* buffer, MHost host)
{
    gui->driver = driver;
    gui->buffer = buffer;
    gui->draw = 0;
    buffer->height = driver->lcd_height;
    buffer->width = driver->lcd_width;
    driver->gui = gui;
    buffer->gui = gui;
    
    buffer->depthmask = MAKISEGUI_BUFFER_DEPTHMASK;
    buffer->pixeldepth = MAKISEGUI_BUFFER_DEPTH;
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
uint8_t makise_start(MakiseGUI * gui)
{
    assert_param(gui == 0);
    assert_param(gui->driver == 0);

    gui->driver->start(gui);
}

uint32_t kpset, kpset32, kpsett;
inline uint32_t makise_pget(MakiseBuffer *b, uint16_t x, uint16_t y)
{
    if((x) < b->width && (y) < (b)->height)		
    {							
	kpset = ((y)*((b)->width) + (x)) * (b)->pixeldepth;
	kpset32 = kpset/32;
	return (b)->depthmask & ((b)->buffer[kpset32] >> (kpset-(kpset32)*32));
    }						
    return 0;
}

inline void makise_pset(MakiseBuffer *b, uint16_t x, uint16_t y, uint32_t c)
{
    if((x) < (b)->width && (y) < (b)->height)				
    {									
	kpset = ((y)*((b)->width) + (x)) * (b)->pixeldepth;
	kpset32 = kpset/32;
	kpsett = kpset - kpset32*32;

	if((b)->depthmask & ((b)->buffer[kpset32] >> (kpsett)) & c)
	    return;
	
	(b)->buffer[kpset32] = ((b)->buffer[kpset32] & ~((b)->depthmask << kpsett)) | (c<<kpsett); 
    }
}
    
