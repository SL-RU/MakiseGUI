#include "makise_text.h"

static void _makise_draw_char(MakiseBuffer *b, uint16_t ind, uint16_t x, uint16_t y, const MakiseFont *font, uint32_t c, uint16_t width)
{
    uint32_t bitCounter, rawIndex, colIndex;
    const uint8_t * ptrByte;

    if(b->border.ex + b->border.w < x ||
	b->border.ey + b->border.h < y ||
       x <= b->border.x - width ||
       y <= b->border.y - font->height)
	return;
    
    ptrByte = &font->table[font->char_index[ind]];
    for(rawIndex = 0; rawIndex < font->height; rawIndex++)
    {
	bitCounter = 0;
	for(colIndex = 0; colIndex < width; colIndex++)
	{

	    if (bitCounter > 7)
	    {
		bitCounter = 0;
		ptrByte++;
	    }
	    if(*ptrByte & (1<<bitCounter))
	    {
		makise_pset(b, x+colIndex, y+rawIndex, c);
	    }
	    bitCounter++;
	}
	ptrByte++;
    }
}

void makise_d_char(MakiseBuffer *b, uint16_t ch, uint16_t x, uint16_t y, const MakiseFont *font, uint32_t c)
{
    uint32_t width;

    ch = (uint8_t)ch - font->offset;
    
    // Symbol width
    if (ch > font->num_char) ch = 0;
    width = font->width ? font->width : font->char_width[ch];
	
    // Draw Char
    _makise_draw_char(b, ch, x, y, font, c, width);
}
void makise_d_string(MakiseBuffer *b, char *s, uint32_t len, uint16_t x, uint16_t y, MDTextPlacement place, const MakiseFont *font, uint32_t c)
{
    uint32_t width, i = 0;

    
    if(place == MDTextPlacement_Center )
    {
	width = makise_d_string_width(s, len, font);
	x -= width / 2;
	y -= font->height / 2;
    } else if(place == MDTextPlacement_CenterUp )
    {
	width = makise_d_string_width(s, len, font);
	x -= width / 2;
    } else if(place == MDTextPlacement_CenterDown )
    {
	width = makise_d_string_width(s, len, font);
	x -= width / 2;
	y -= font->height;
    }
    
    uint32_t ch, xt = x, yt = y;
    
    while (i < len && s[i]) {
	ch = s[i];
	ch = (uint8_t)ch - font->offset;

	// Symbol width
	if (ch > font->num_char) ch = 0;
	width = font->width ? font->width : font->char_width[ch];
	
	// Draw Char
	_makise_draw_char(b, ch, xt, yt, font, c, width);
	xt += width + font->space_char;
	i++;
    }
}

uint32_t makise_d_string_width(char *s, uint32_t len, const MakiseFont *font)
{
    uint32_t width , i = 0;
    uint32_t ch, res = 0;
    
    while (i < len && s[i]) {
	ch = s[i];
	ch = (uint8_t)ch - font->offset;

	// Symbol width
	if (ch > font->num_char) ch = 0;
	width = font->width ? font->width : font->char_width[ch];	
	res += width + font->space_char;
	
	i++;
    }
    return res;
}

//draw multiline text in the defined frame
void makise_d_string_frame(MakiseBuffer *b, char *s, uint32_t len, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const MakiseFont *font, uint16_t line_spacing, uint32_t c)
{
    uint32_t width, i = 0;

    uint32_t ch, xt = x, yt = y;

    
    while (i < len && s[i])
    {
	if(s[i] == '\n' || s[i] == '\r')
	{
	    xt = x;
	    yt += font->height + line_spacing;
	    if(yt + font->height > y + h)
		return;
	}
	else
	{
	    ch = s[i];
	    ch = (uint8_t)ch - font->offset;

	    // Symbol width
	    if (ch > font->num_char) ch = 0;
	    width = font->width ? font->width : font->char_width[ch];

	    if(xt + width > x + w)
	    {
		xt = x;
		
		yt += font->height + line_spacing;
		if(yt + font->height > y + h)
		    return;
	    }
	    // Draw Char
	    _makise_draw_char(b, ch, xt, yt, font, c, width);
	    xt += width + font->space_char;
	}
	i++;
    }

}
