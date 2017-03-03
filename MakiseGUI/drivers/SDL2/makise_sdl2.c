#include "makise_config.h"
#if MAKISE_SDL2_USE == 1
#include "makise_sdl2.h"

SDL_Renderer *_makise_sdl2_renderer;

void makise_sdl2_driver(MakiseDriver* d, uint32_t width, uint32_t height, SDL_Renderer* rnd)
{
    d->lcd_height    = height;
    d->lcd_width     = width;
    d->buffer_height = MAKISE_BUF_H;
    d->buffer_width  = MAKISE_BUF_W;
    d->pixeldepth    = 16;
    d->buffer        = 0;
    d->size          = 0;
    d->posx          = 0;
    d->posy          = 0;
    d->init          = &makise_sdl2_init;
    d->start         = &makise_sdl2_start;
    d->sleep         = &makise_sdl2_sleep;
    d->awake         = &makise_sdl2_awake;
    d->set_backlight = &makise_sdl2_set_backlight;

    _makise_sdl2_renderer = rnd;
}

uint8_t makise_sdl2_init (MakiseGUI* gui)
{
    return M_OK;
}
uint8_t makise_sdl2_start(MakiseGUI* gui)
{
    return M_OK;
}
uint8_t makise_sdl2_sleep(MakiseGUI* gui)
{
    return M_OK;
}
uint8_t makise_sdl2_awake(MakiseGUI* gui)
{
    return M_OK;
}
uint8_t makise_sdl2_set_backlight(MakiseGUI* gui, uint8_t b)
{
    return M_OK;
}

void makise_sdl2_draw(MakiseGUI* gui)
{
    MakiseDriver * d = gui->driver;
    uint16_t c;

    if(d->gui->draw != 0)
    {
	d->gui->draw(d->gui);
    }

    uint32_t y = 0      , //start position y
	x = 0,
	cu = 0,
	bu = 0;
    
    for (; y < d->lcd_height; y++) {	
	for (x = 0; x < d->lcd_width; x+=1)
	{
//	    c = makise_color_get(((((uint32_t*)gui->buffer->buffer)[cu]) >> bu) & gui->buffer->depthmask);
	    c = makise_pget(gui->buffer, x, y);
	    bu += gui->buffer->pixeldepth;
	    if(bu>=32)
	    {
		bu = 0;
		cu ++;
	    }

	    uint16_t R8 = ((c >> 11)             * 8 );
	    uint16_t G8 = (((c >> 5) & 0b111111) * 4 );
	    uint16_t B8 = (((c     ) & 0b11111 ) * 8 );
//	    printf("%d", B8);
	    
	    SDL_SetRenderDrawColor(_makise_sdl2_renderer, R8, G8, B8, 255);
	    SDL_RenderDrawPoint(_makise_sdl2_renderer, x, y);

	}
    }
//    printf("f\n");
    memset(gui->buffer->buffer, 0, 960000);

    
    if(d->gui->predraw != 0)
    {
	d->gui->predraw(d->gui);
    }
    if(d->gui->update != 0)
    {
	d->gui->update(d->gui);
    }

}

#endif
