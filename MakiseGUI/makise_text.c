#include "makise_text.h"
#include <stdio.h>

static void _makise_draw_char(const MakiseBuffer *b,
                              uint16_t ind, int16_t x, int16_t y,
                              const MakiseFont *font, MColor c, uint16_t width)
{
    uint32_t bitCounter, rawIndex, colIndex;
    const uint8_t * ptrByte;

    if((int16_t)(b->border.ex + b->border.w) < x ||
       (int16_t)(b->border.ey + b->border.h) < y ||
       x <= b->border.x - width ||
       y <= b->border.y - font->height)
	return;

    
    ptrByte = &font->table[font->char_index[ind]];
    bitCounter = 0;
    for(rawIndex = 0; rawIndex < font->height; rawIndex++)
    {
	for(colIndex = 0; colIndex < width; colIndex++)
	{
	    if (bitCounter > 7)
	    {
		bitCounter = 0;
		ptrByte++;
	    }
	    if(*ptrByte & (1<<bitCounter))
	    {
		makise_d_point(b, (MPoint){x+colIndex, y+rawIndex}, c);
	    }
	    bitCounter++;
	}
    }
}

void makise_d_char(const MakiseBuffer *b,
                   uint16_t ch, int16_t x, int16_t y,
                   const MakiseFont *font, MColor c)
{
    if(b == 0) return;
    
    if(b->text_drawer != 0 && b->text_drawer->d_char != 0) {
        b->text_drawer->d_char(b, ch, x, y, font, c);
        return;
    }
    
    uint32_t width;

    ch = (uint8_t)ch - font->offset;
    
    // Symbol width
    if (ch > font->num_char) ch = 0;
    width = font->width ? font->width : font->char_width[ch];

    // Draw Char
    _makise_draw_char(b, ch, x, y, font, c, width);
}

void makise_d_string(const MakiseBuffer *b,
                     const char *s, uint32_t len,
                     int16_t x, int16_t y, MDTextPlacement place,
                     const MakiseFont *font, MColor c)
{
    if(b == 0) return;
    
    if(b->text_drawer != 0 && b->text_drawer->string != 0) {
        b->text_drawer->string(b, s, len, x, y, place, font, c);
        return;
    }
    
    uint32_t width, i = 0;

    if(s == 0)
	return;
    
    if(place & MDTextPlacement_HCenter ) {
	width = makise_d_string_get_width(b, s, len, font);
	x -= width / 2;
    } else if(place & MDTextPlacement_Right) {
	width = makise_d_string_get_width(b, s, len, font);
	x -= width;
    }
    
    if(place & MDTextPlacement_Down ) {
	y -= font->height;
    } else if(place & MDTextPlacement_VCenter ) {
	y -= font->height / 2;
    }

   
    int32_t ch, xt = x, yt = y;

    if(y + font->height < b->border.y ||
       y > b->border.ey) //borders
	return;
    
    while (i < len && s[i]) {
#if MAKISE_UNICODE
	uint8_t bts = 0;
	ch = makise_d_utf_char_id(&s[i], len - i, &bts);
	ch = makise_d_utf_char_font(ch, font);
	if (ch > font->num_uni) ch = 0;
	i += bts;
#else
	ch = s[i];
	ch = (uint8_t)ch - font->offset;
	if (ch > font->num_char) ch = 0;
	i++;
#endif
	// Symbol width
	width = font->width ? font->width : font->char_width[ch];

	// Draw Char
	_makise_draw_char(b, ch, xt, yt, font, c, width);
	xt += width + font->space_char;

	if(xt >= (int32_t)b->border.ex) // right border
	    return;
    }
}

uint32_t makise_d_string_get_width(const MakiseBuffer *b,
                                   const char *s,
                                   uint32_t len,
                                   const MakiseFont *font)
{
    if(s == 0)
	return 0;
    
    if(b->text_drawer != 0 && b->text_drawer->get_width != 0) {
        return b->text_drawer->get_width(b, s, len, font);
    }
    uint32_t width , i = 0;
    uint32_t ch, res = 0;

    
    while (i < len && s[i]) {
#if MAKISE_UNICODE
	uint8_t bts = 0;
	ch = makise_d_utf_char_id(&s[i], len - i, &bts);
	ch = makise_d_utf_char_font(ch, font);
	if (ch > font->num_uni) ch = 0;
	i += bts;
#else
	ch = s[i];
	ch = (uint8_t)ch - font->offset;
	if (ch > font->num_char) ch = 0;
	i++;
#endif

	// Symbol width
	//if (ch > font->num_char) ch = 0;
	width = font->width ? font->width : font->char_width[ch];
	res += width + font->space_char;

    }
    return res;
}

uint32_t makise_d_string_height_get ( const MakiseBuffer *b,
                                      const char*       s,
                                      uint32_t          len,
                                      uint16_t          width_window,
                                      const MakiseFont* font,
                                      uint32_t          font_line_spacing )
{
    if(s == 0)
	return 0;
    
    if(b->text_drawer != 0 && b->text_drawer->get_height != 0) {
        return b->text_drawer->get_height(b, s, len,
                                          width_window,
                                          font, font_line_spacing);
    }
    
    uint32_t i = 0;
    uint32_t height = font->height;
    uint32_t width = 0;
    while ( i < len && s[i] ) {

#if MAKISE_UNICODE
        uint8_t bts = 0;
        uint32_t ch;
        ch = makise_d_utf_char_id( &s[i], len - i, &bts );
        ch = makise_d_utf_char_font(ch, font);
        if ( ch > font->num_uni ) ch = 0;
        i += bts;
#else
        ch = s[i];
        ch = (uint8_t)ch - font->offset;
        if (ch > font->num_char) ch = 0;
        i++;
#endif
        uint32_t width_char = font->width ? font->width : font->char_width[ch];

        if ( width + width_char + font->space_char > width_window ) {
            width = width_char;
            height += font->height + font_line_spacing;
        } else {
            width += width_char + font->space_char;
        }
    }
    return height;
}

/**
 * Get total line count of the text in the frame with selected width.
 * Methods calculates line wraps and new line returns.
 *
 * @param s text
 * @param len length of text
 * @param w width of frame
 * @param font 
 * @return count of text lines
 */
uint32_t    makise_d_string_get_line_count ( const MakiseBuffer *b,
                                             const char *s,
                                             uint32_t len,
                                             uint16_t w,
                                             const MakiseFont *font )
{
    if(s == 0)
	return 0;
    
    if(b->text_drawer != 0 && b->text_drawer->get_line_count != 0) {
        return b->text_drawer->get_line_count(b, s, len, w, font);
    }
    uint32_t width, i = 0;

    uint32_t ch, xt = 0, lines = 1;

    while ( i < len && s[i] ) {
	if(s[i] == '\n' || s[i] == '\r')
	{
	    //calculate return
	    xt = 0;
	    lines ++;
	    i++;
	}
	else
	{
#if MAKISE_UNICODE
	    uint8_t bts = 0;
	    ch = makise_d_utf_char_id( &s[i], len - i, &bts );
	    ch = makise_d_utf_char_font(ch, font);
	    if ( ch > font->num_uni ) ch = 0;
	    i += bts;
#else
	    ch = s[i];
	    ch = (uint8_t)ch - font->offset;
	    if (ch > font->num_char) ch = 0;
	    i++;
#endif
	    width = font->width ? font->width : font->char_width[ch];
	    
	    if(xt + width > w)
	    {
		xt = 0;
		lines ++;
	    }
	    xt += width + font->space_char;
	}
    }
    return lines;
}


/**
 * Returns pointer to the n's line. It calculates returns, wraps and etc.
 *
 * @param s string
 * @param len string's len
 * @param line required line
 * @param w width of frame
 * @param font font
 * @return pointer tobeginning og n's line
 */
char *     makise_d_string_get_line ( const MakiseBuffer *b,
                                      char *s,
                                      uint32_t len,
                                      uint32_t n,
                                      uint16_t w,
                                      const MakiseFont *font )
{
    if(s == 0)
	return 0;
    
    if(b->text_drawer != 0 && b->text_drawer->get_line != 0) {
        return b->text_drawer->get_line(b, s, len, n, w, font);
    }
    uint32_t width, i = 0;

    uint32_t ch, xt = 0, lines = 0;

    
    while ( i < len && s[i] ) {
	if(s[i] == '\n' || s[i] == '\r')
	{
	    //calculate return
	    xt = 0;
	    lines ++;
	    i++;
	    if(lines == n)
		return &s[i];
	}
	else
	{
#if MAKISE_UNICODE
	    uint8_t bts = 0;
	    ch = makise_d_utf_char_id( &s[i], len - i, &bts );
	    ch = makise_d_utf_char_font(ch, font);
	    if ( ch > font->num_uni ) ch = 0;
#else
	    ch = s[i];
	    ch = (uint8_t)ch - font->offset;
	    if (ch > font->num_char) ch = 0;
#endif
	    width = font->width ? font->width : font->char_width[ch];
	    
	    if(xt + width > w)
	    {
		xt = 0;
		lines ++;
		if(lines == n)
		    return &s[i];
	    }
#if MAKISE_UNICODE
	    i += bts;
#else
	    i++;
#endif

	    xt += width + font->space_char;
	}
    }
    return 0;
}

//draw multiline text in the defined frame
void makise_d_string_frame(const MakiseBuffer *b,
			   const char *s, uint32_t len,
			   int16_t x, int16_t y, uint16_t w, uint16_t h,
			   const MakiseFont *font, uint16_t line_spacing, MColor c)
{
    int32_t width, i = 0;

    int32_t ch, xt = x, yt = y;

    if(s == 0)
	return;

    
    while (i < len && s[i])
    {
	if(s[i] == '\n' || s[i] == '\r')
	{
	    xt = x;
	    yt += font->height + line_spacing;
	    i++;
	    if(yt + font->height > y + h)
		return;
	}
	else
	{
#if MAKISE_UNICODE
	    uint8_t bts = 0;
	    ch = makise_d_utf_char_id(&s[i], len - i, &bts);
	    ch = makise_d_utf_char_font(ch, font);
	    if (ch > font->num_uni) ch = 0;
	    i += bts;
#else
	    ch = s[i];
	    ch = (uint8_t)ch - font->offset;
	    if (ch > font->num_char) ch = 0;
	    i++;
#endif

	    // Symbol width
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
    }

}

#if MAKISE_UNICODE
/* static const struct validUTF8Sequence */
/* { */
/*     uint32_t  lowChar; */
/*     uint32_t highChar; */
/*     uint16_t numBytes; */
/*     uint8_t validBytes[8]; */
/* } validUTF8[7] = */
/* { */
/* /\*   low       high   #bytes  byte 1      byte 2      byte 3      byte 4 *\/ */
/*     {0x0000,   0x007F,   1, {0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}, */
/*     {0x0080,   0x07FF,   2, {0xC2, 0xDF, 0x80, 0xBF, 0x00, 0x00, 0x00, 0x00}}, */
/*     {0x0800,   0x0FFF,   3, {0xE0, 0xE0, 0xA0, 0xBF, 0x80, 0xBF, 0x00, 0x00}}, */
/*     {0x1000,   0xFFFF,   3, {0xE1, 0xEF, 0x80, 0xBF, 0x80, 0xBF, 0x00, 0x00}}, */
/*     {0x10000,  0x3FFFF,  4, {0xF0, 0xF0, 0x90, 0xBF, 0x80, 0xBF, 0x80, 0xBF}}, */
/*     {0x40000,  0xFFFFF,  4, {0xF1, 0xF3, 0x80, 0xBF, 0x80, 0xBF, 0x80, 0xBF}}, */
/*     {0x100000, 0x10FFFF, 4, {0xF4, 0xF4, 0x80, 0x8F, 0x80, 0xBF, 0x80, 0xBF}}  */
/* }; */

uint32_t makise_d_utf_char_id( const char *s, uint32_t len, uint8_t *bts)
{
    if(len == 0)
    {
	*bts = 0;
	return 0;
    }
    if(((uint8_t*)s)[0] < 128)
    {
	*bts = 1;
	return s[0];
    }

    uint8_t ch = s[0];
    uint32_t n = 0;
    uint16_t bytes = 0, i;
    uint8_t hasError = 0;
    
    
    if (ch <= 0x7F) /* 0XXX XXXX one byte */
    {
        n = ch;
        bytes = 1;
    }
    else if ((ch & 0xE0) == 0xC0)  /* 110X XXXX  two bytes */
    {
        n = ch & 31;
        bytes = 2;
    }
    else if ((ch & 0xF0) == 0xE0)  /* 1110 XXXX  three bytes */
    {
        n = ch & 15;
        bytes = 3;
    }
    else if ((ch & 0xF8) == 0xF0)  /* 1111 0XXX  four bytes */
    {
        n = ch & 7;
        bytes = 4;
    }
    else if ((ch & 0xFC) == 0xF8)  /* 1111 10XX  five bytes */
    {
        n = ch & 3;
        bytes = 5;
        hasError = 1;
    }
    else if ((ch & 0xFE) == 0xFC)  /* 1111 110X  six bytes */
    {
        n = ch & 1;
        bytes = 6;
        hasError = 1;
    }
    else
    {
        /* not a valid first byte of a UTF-8 sequence */
        n = ch;
        bytes = 1;
        hasError = 1;
    }

    if(len < bytes)
    {
	*bts = 1;
	return s[0];
    }
    
    for ( i=1; i < bytes; i++ )
    {
	if ( !s[i] || (s[i] & 0xC0) != 0x80 )
	{
	    hasError = 1;
	    bytes = i;
	    break;
	}
	n = (n << 6) | (s[i] & 0x3F);
    }

    *bts = bytes;
    
    if(hasError)
	return 0;

    return n;
    
}

uint32_t    makise_d_utf_char_font  ( uint32_t c, const MakiseFont *font)
{
    //if character is in the first block
    if(c < font->offset + font->num_char)
	return c - font->offset;

    for (uint16_t i = 0; i < font->num_uni - font->num_char; i++) {
	if((uint16_t)c == font->unicode_index[i])
	{
	    //printf("f %d %d\n", c, i);
	    return i + font->num_char;
	}
    }
    return UINT32_MAX;
}


uint32_t makise_d_utf_char_encode(uint32_t code, uint8_t *utf8)
{
    uint32_t tail = 0;
    uint32_t i = 0;
    static const uint8_t utf8comp[6] =	{
	0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC
    };

    if (code > 0x7F)
        while (code >> (5*tail + 6))
            tail++;

    utf8[i++] = (code >> (6*tail)) | utf8comp[tail];
    while (tail--)
        utf8[i++] = ((code >> (6*tail)) &
		     0x3F) | 0x80; //mask

    return i;
}

//#define getbe16(p) ((p[0] << 8) | p[1])


uint32_t makise_d_utf16_string_decode(uint8_t *utf16,
				      uint8_t *utf8, uint32_t count)
{
    uint32_t code, len = 0;

    while (count > 0) {
        /* Check for a surrogate pair */

        code = utf16[0] << 0 | (utf16[1] << 8);
	utf16 += 2;
	count -= 2;
        len += makise_d_utf_char_encode(code, utf8 + len);
    }
    return len;
}

/* /\* Recode a UTF-16 string with big-endian byte ordering to UTF-8 *\/ */
/* unsigned char* utf16BEdecode(const unsigned char *utf16, unsigned char *utf8, */
/*         int count) */
/* { */
/*     unsigned long code; */

/*     while (count > 0) { */
/*         if (*utf16 >= 0xD8 && *utf16 < 0xE0) { /\* Check for a surrogate pair *\/ */
/*             code = 0x10000 + (((utf16[0] - 0xD8) << 18) | (utf16[1] << 10) */
/*                     | ((utf16[2] - 0xDC) << 8) | utf16[3]); */
/*             utf16 += 4; */
/*             count -= 2; */
/*         } else { */
/*             code = getbe16(utf16); */
/*             utf16 += 2; */
/*             count -= 1; */
/*         } */
/*         utf8 = utf8encode(code, utf8); */
/*     } */
/*     return utf8; */
/* } */

/* #if 0 /\* currently unused *\/ */
/* /\* Recode any UTF-16 string to UTF-8 *\/ */
/* unsigned char* utf16decode(const unsigned char *utf16, unsigned char *utf8, */
/*         unsigned int count) */
/* { */
/*     uAnsigned long code; */

/*     code = *(utf16++) << 8; */
/*     code |= *(utf16++); */

/*     if (code == 0xFEFF) /\* Check for BOM *\/ */
/*         return utf16BEdecode(utf16, utf8, count-1); */
/*     else if (code == 0xFFFE) */
/*         return utf16LEdecode(utf16, utf8, count-1); */
/*     else { /\* ADDME: Should default be LE or BE? *\/ */
/*         utf16 -= 2; */
/*         return utf16BEdecode(utf16, utf8, count); */
/*     } */
/* } */
/* #endif */

#endif //unicode
