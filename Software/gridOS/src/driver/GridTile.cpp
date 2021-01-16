/*
 * GridTile.cpp
 *
 *  Created on: 28.03.2019
 *      Author: pmale
 */
#include "GridTile.h"
#include "fonts/glcdfont.cpp"
#include "fonts/PicoPixel.h"

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif
#ifndef pgm_read_byte
 #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif
#ifndef pgm_read_word
 #define pgm_read_word(addr) (*(const unsigned short *)(addr))
#endif
#ifndef pgm_read_dword
 #define pgm_read_dword(addr) (*(const unsigned long *)(addr))
#endif

static const char* TAG = "GridTile";

GridTile::GridTile(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint8_t rot)
{
    if(w == 0 || h == 0){
        ESP_LOGE(TAG,"width or height set to 0!");
        return;
    }
    Grid::init();	
    this->TileX0 = x0;
	this->TileY0 = y0;
	this->TileX1 = x0+(w-1);
	this->TileY1 = y0+(h-1);
    ESP_LOGI(TAG,"Bounds x%i-%i y%i-%i",TileX0,TileX1,TileY0,TileY1);
    setRotation(rot);
    cursor_y  = cursor_x    = 0;
    textsize_x = textsize_y  = 1;
    wrap      = true;
    _cp437    = false;
    gridFont   = NULL;
    setFont(&Picopixel);
}

uint8_t GridTile::getWidth()
{
	return (TileX1-TileX0)+1;
}

uint8_t GridTile::getHeight()
{
	return (TileY1-TileY0)+1;
}

void GridTile::writePixel(int16_t x, int16_t y, CRGBW color)
{
    int16_t rot_x = -1, rot_y = -1;

    x+=TileX0;  //shift input coords to grid coords
    y+=TileY0;

    if(x > TileX1 || y > TileY1){
        return;
    }

    switch (rotation)
    {
    case 0: //TOP LEFT
        rot_x = x; //no rotation
        rot_y = y;
        break;
    case 1: //TOP RIGHT, x -> y, y -> width-x
        rot_x = GRID_WIDTH-y-1;
        rot_y = x;
        break;
    case 2: //BOT RIGHT, x -> width-x, y -> height-y
        rot_x = GRID_WIDTH-x-1;
        rot_y = GRID_HEIGHT-y-1;
        break;
    case 3: //BOT LEFT, x -> height
        rot_x = y;
        rot_y = GRID_HEIGHT-x-1;
        break;
    }
    if(rot_x < TileX0 || rot_y < TileY0 || rot_x > TileX1 || rot_y > TileY1)
	{  //if draw not is inside tile dimensions, discard it
        //ESP_LOGW(TAG,"OOB x=%i, y=%i",x,y);
		return;
	}
	Grid::writePixel((uint16_t)rot_x, (uint16_t)rot_y, color);
}

void GridTile::fillScreen(CRGBW color)
{
	fillScreenBuffer(color);
	endWrite();
}

void GridTile::fillScreenBuffer(CRGBW color) {
	for(uint16_t x = 0; x < getWidth(); x++)
		{
			for(uint16_t y = 0; y < getHeight(); y++)
			{
				writePixel(x, y, color);
			}
		}
}

void GridTile::endWrite()
{
	Grid::endWrite();
}

uint8_t GridTile::getRotation()
{
    return rotation;
}
void GridTile::setRotation(uint8_t rot)
{
    if(rot <= 3)
    {
        rotation = rot;
    }
}

void GridTile::writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGBW color)
{
    int16_t steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        _swap_int16_t(x0, y0);
        _swap_int16_t(x1, y1);
    }

    if (x0 > x1) {
        _swap_int16_t(x0, x1);
        _swap_int16_t(y0, y1);
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }

    for (; x0<=x1; x0++) {
        if (steep) {
            writePixel(y0, x0, color);
        } else {
            writePixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

void GridTile::writeRect(int16_t x, int16_t y, int16_t w, int16_t h, CRGBW color)
{
    writeFastHLine(x, y, w, color);
    writeFastHLine(x, y+h-1, w, color);
    writeFastVLine(x, y, h, color);
    writeFastVLine(x+w-1, y, h, color);
}

void GridTile::writeFilledRect(int16_t x, int16_t y, int16_t w, int16_t h, CRGBW color)
{
    for (int16_t i=x; i<x+w; i++) {
        writeFastVLine(i, y, h, color);
    }
}

void GridTile::writeFastVLine(int16_t x, int16_t y, int16_t h, CRGBW color)
{
	writeLine(x, y, x, y+h-1, color);
}

void GridTile::writeFastHLine(int16_t x, int16_t y, int16_t w, CRGBW color)
{
	writeLine(x, y, x+w-1, y, color);
}

/**************************************************************************/
/*!
   @brief    Draw a circle outline
    @param    x0   Center-point x coordinate
    @param    y0   Center-point y coordinate
    @param    r   Radius of circle
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void GridTile::writeCircle(int16_t x0, int16_t y0, int16_t r, CRGBW color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    writePixel(x0  , y0+r, color);
    writePixel(x0  , y0-r, color);
    writePixel(x0+r, y0  , color);
    writePixel(x0-r, y0  , color);

    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        writePixel(x0 + x, y0 + y, color);
        writePixel(x0 - x, y0 + y, color);
        writePixel(x0 + x, y0 - y, color);
        writePixel(x0 - x, y0 - y, color);
        writePixel(x0 + y, y0 + x, color);
        writePixel(x0 - y, y0 + x, color);
        writePixel(x0 + y, y0 - x, color);
        writePixel(x0 - y, y0 - x, color);
    }
}

/**************************************************************************/
/*!
    @brief    Quarter-circle drawer, used to do circles and roundrects
    @param    x0   Center-point x coordinate
    @param    y0   Center-point y coordinate
    @param    r   Radius of circle
    @param    cornername  Mask bit #1 or bit #2 to indicate which quarters of the circle we're doing
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void GridTile::drawCircleHelper( int16_t x0, int16_t y0, int16_t r, uint8_t cornername, CRGBW color) {
    int16_t f     = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x     = 0;
    int16_t y     = r;

    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f     += ddF_y;
        }
        x++;
        ddF_x += 2;
        f     += ddF_x;
        if (cornername & 0x4) {
            writePixel(x0 + x, y0 + y, color);
            writePixel(x0 + y, y0 + x, color);
        }
        if (cornername & 0x2) {
            writePixel(x0 + x, y0 - y, color);
            writePixel(x0 + y, y0 - x, color);
        }
        if (cornername & 0x8) {
            writePixel(x0 - y, y0 + x, color);
            writePixel(x0 - x, y0 + y, color);
        }
        if (cornername & 0x1) {
            writePixel(x0 - y, y0 - x, color);
            writePixel(x0 - x, y0 - y, color);
        }
    }
}

/**************************************************************************/
/*!
   @brief    Draw a circle with filled color
    @param    x0   Center-point x coordinate
    @param    y0   Center-point y coordinate
    @param    r   Radius of circle
    @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void GridTile::writeFilledCircle(int16_t x0, int16_t y0, int16_t r,CRGBW color) {
    writeFastVLine(x0, y0-r, 2*r+1, color);
    fillCircleHelper(x0, y0, r, 3, 0, color);
}


/**************************************************************************/
/*!
    @brief  Quarter-circle drawer with fill, used for circles and roundrects
    @param  x0       Center-point x coordinate
    @param  y0       Center-point y coordinate
    @param  r        Radius of circle
    @param  corners  Mask bits indicating which quarters we're doing
    @param  delta    Offset from center-point, used for round-rects
    @param  color    16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void GridTile::fillCircleHelper(int16_t x0, int16_t y0, int16_t r,uint8_t corners, int16_t delta, CRGBW color) {

    int16_t f     = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x     = 0;
    int16_t y     = r;
    int16_t px    = x;
    int16_t py    = y;

    delta++; // Avoid some +1's in the loop

    while(x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f     += ddF_y;
        }
        x++;
        ddF_x += 2;
        f     += ddF_x;
        // These checks avoid double-drawing certain lines, important
        // for the SSD1306 library which has an INVERT drawing mode.
        if(x < (y + 1)) {
            if(corners & 1) writeFastVLine(x0+x, y0-y, 2*y+delta, color);
            if(corners & 2) writeFastVLine(x0-x, y0-y, 2*y+delta, color);
        }
        if(y != py) {
            if(corners & 1) writeFastVLine(x0+py, y0-px, 2*px+delta, color);
            if(corners & 2) writeFastVLine(x0-py, y0-px, 2*px+delta, color);
            py = y;
        }
        px = x;
    }
}

inline GridGlyph * pgm_read_glyph_ptr(const GridFont *gfxFont, uint8_t c)
{
    return gfxFont->glyph + c;
}

inline uint8_t * pgm_read_bitmap_ptr(const GridFont *gfxFont)
{
    return gfxFont->bitmap;
}

// TEXT- AND CHARACTER-HANDLING FUNCTIONS ----------------------------------

void GridTile::writeString(int16_t x, int16_t y, std::string str, CRGBW color, CRGBW bg, uint8_t size)
{
    uint16_t x_shift = 0;
    std::string::iterator it = str.begin();
	while(it != str.end())
	{
		char c = *it;
		writeChar(x+x_shift,y,c,color,bg,size);
        x_shift +=4*size;
		it++;
	}
}

// Draw a character
/**************************************************************************/
/*!
   @brief   Draw a single character
    @param    x   Bottom left corner x coordinate
    @param    y   Bottom left corner y coordinate
    @param    c   The 8-bit font-indexed character (likely ascii)
    @param    color 16-bit 5-6-5 Color to draw chraracter with
    @param    bg 16-bit 5-6-5 Color to fill background with (if same as color, no background)
    @param    size  Font magnification level, 1 is 'original' size
*/
/**************************************************************************/
void GridTile::writeChar(int16_t x, int16_t y, unsigned char c,
  CRGBW color, CRGBW bg, uint8_t size) {
    writeChar(x, y, c, color, bg, size, size);
}

// Draw a character
/**************************************************************************/
/*!
   @brief   Draw a single character
    @param    x   Bottom left corner x coordinate
    @param    y   Bottom left corner y coordinate
    @param    c   The 8-bit font-indexed character (likely ascii)
    @param    color 16-bit 5-6-5 Color to draw chraracter with
    @param    bg 16-bit 5-6-5 Color to fill background with (if same as color, no background)
    @param    size_x  Font magnification level in X-axis, 1 is 'original' size
    @param    size_y  Font magnification level in Y-axis, 1 is 'original' size
*/
/**************************************************************************/
void GridTile::writeChar(int16_t x, int16_t y, unsigned char c,
  CRGBW color, CRGBW bg, uint8_t size_x, uint8_t size_y) {

    if(!gridFont) { // 'Classic' built-in font

        if((x >= getWidth())            || // Clip right
           (y >= getHeight())           || // Clip bottom
           ((x + 6 * size_x - 1) < 0) || // Clip left
           ((y + 8 * size_y - 1) < 0))   // Clip top
            return;

        if(!_cp437 && (c >= 176)) c++; // Handle 'classic' charset behavior

        for(int8_t i=0; i<5; i++ ) { // Char bitmap = 5 columns
            uint8_t line = pgm_read_byte(&font[c * 5 + i]);
            for(int8_t j=0; j<8; j++, line >>= 1) {
                if(line & 1) {
                    if(size_x == 1 && size_y == 1)
                        writePixel(x+i, y+j, color);
                    else
                        writeFilledRect(x+i*size_x, y+j*size_y, size_x, size_y, color);
                } else if(bg != color) {
                    if(size_x == 1 && size_y == 1)
                        writePixel(x+i, y+j, bg);
                    else
                        writeFilledRect(x+i*size_x, y+j*size_y, size_x, size_y, bg);
                }
            }
        }
        if(bg != color) { // If opaque, draw vertical line for last column
            if(size_x == 1 && size_y == 1) writeFastVLine(x+5, y, 8, bg);
            else          writeFilledRect(x+5*size_x, y, size_x, 8*size_y, bg);
        }

    } else { // Custom font

        // Character is assumed previously filtered by write() to eliminate
        // newlines, returns, non-printable characters, etc.  Calling
        // drawChar() directly with 'bad' characters of font may cause mayhem!

        c -= (uint8_t)pgm_read_byte(&gridFont->first);
        GridGlyph *glyph  = pgm_read_glyph_ptr(gridFont, c);
        uint8_t  *bitmap = pgm_read_bitmap_ptr(gridFont);

        uint16_t bo = pgm_read_word(&glyph->bitmapOffset);
        uint8_t  w  = pgm_read_byte(&glyph->width),
                 h  = pgm_read_byte(&glyph->height);
        int8_t   xo = pgm_read_byte(&glyph->xOffset),
                 yo = pgm_read_byte(&glyph->yOffset);
        uint8_t  xx, yy, bits = 0, bit = 0;
        int16_t  xo16 = 0, yo16 = 0;

        if(size_x > 1 || size_y > 1) {
            xo16 = xo;
            yo16 = yo;
        }

        // Todo: Add character clipping here

        // NOTE: THERE IS NO 'BACKGROUND' COLOR OPTION ON CUSTOM FONTS.
        // THIS IS ON PURPOSE AND BY DESIGN.  The background color feature
        // has typically been used with the 'classic' font to overwrite old
        // screen contents with new data.  This ONLY works because the
        // characters are a uniform size; it's not a sensible thing to do with
        // proportionally-spaced fonts with glyphs of varying sizes (and that
        // may overlap).  To replace previously-drawn text when using a custom
        // font, use the getTextBounds() function to determine the smallest
        // rectangle encompassing a string, erase the area with fillRect(),
        // then draw new text.  This WILL infortunately 'blink' the text, but
        // is unavoidable.  Drawing 'background' pixels will NOT fix this,
        // only creates a new set of problems.  Have an idea to work around
        // this (a canvas object type for MCUs that can afford the RAM and
        // displays supporting setAddrWindow() and pushColors()), but haven't
        // implemented this yet.

        for(yy=0; yy<h; yy++) {
            for(xx=0; xx<w; xx++) {
                if(!(bit++ & 7)) {
                    bits = pgm_read_byte(&bitmap[bo++]);
                }
                if(bits & 0x80) {
                    if(size_x == 1 && size_y == 1) {
                        writePixel(x+xo+xx, y+yo+yy, color);
                    } else {
                        writeFilledRect(x+(xo16+xx)*size_x, y+(yo16+yy)*size_y,
                          size_x, size_y, color);
                    }
                }
                bits <<= 1;
            }
        }

    } // End classic vs custom font
}

/**************************************************************************/
/*!
    @brief   Set text 'magnification' size. Each increase in s makes 1 pixel that much bigger.
    @param  s  Desired text size. 1 is default 6x8, 2 is 12x16, 3 is 18x24, etc
*/
/**************************************************************************/
void GridTile::setTextSize(uint8_t s) {
    setTextSize(s, s);
}

/**************************************************************************/
/*!
    @brief   Set text 'magnification' size. Each increase in s makes 1 pixel that much bigger.
    @param  s_x  Desired text width magnification level in X-axis. 1 is default
    @param  s_y  Desired text width magnification level in Y-axis. 1 is default
*/
/**************************************************************************/
void GridTile::setTextSize(uint8_t s_x, uint8_t s_y) {
    textsize_x = (s_x > 0) ? s_x : 1;
    textsize_y = (s_y > 0) ? s_y : 1;
}

/**************************************************************************/
/*!
    @brief Set the font to display when print()ing, either custom or default
    @param  f  The GFXfont object, if NULL use built in 6x8 font
*/
/**************************************************************************/
void GridTile::setFont(const GridFont *f) {
    if(f) {            // Font struct pointer passed in?
        if(!gridFont) { // And no current font struct?
            // Switching from classic to new font behavior.
            // Move cursor pos down 6 pixels so it's on baseline.
            cursor_y += 6;
        }
    } else if(gridFont) { // NULL passed.  Current font struct defined?
        // Switching from new to classic font behavior.
        // Move cursor pos up 6 pixels so it's at top-left of char.
        cursor_y -= 6;
    }
    gridFont = (GridFont *)f;
}


/**************************************************************************/
/*!
    @brief    Helper to determine size of a character with current font/size.
       Broke this out as it's used by both the PROGMEM- and RAM-resident getTextBounds() functions.
    @param    c     The ascii character in question
    @param    x     Pointer to x location of character
    @param    y     Pointer to y location of character
    @param    minx  Minimum clipping value for X
    @param    miny  Minimum clipping value for Y
    @param    maxx  Maximum clipping value for X
    @param    maxy  Maximum clipping value for Y
*/
/**************************************************************************/
void GridTile::charBounds(char c, int16_t *x, int16_t *y,
  int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy) {

    if(gridFont) {

        if(c == '\n') { // Newline?
            *x  = 0;    // Reset x to zero, advance y by one line
            *y += textsize_y * (uint8_t)pgm_read_byte(&gridFont->yAdvance);
        } else if(c != '\r') { // Not a carriage return; is normal char
            uint8_t first = pgm_read_byte(&gridFont->first),
                    last  = pgm_read_byte(&gridFont->last);
            if((c >= first) && (c <= last)) { // Char present in this font?
                GridGlyph *glyph = pgm_read_glyph_ptr(gridFont, c - first);
                uint8_t gw = pgm_read_byte(&glyph->width),
                        gh = pgm_read_byte(&glyph->height),
                        xa = pgm_read_byte(&glyph->xAdvance);
                int8_t  xo = pgm_read_byte(&glyph->xOffset),
                        yo = pgm_read_byte(&glyph->yOffset);
                if(wrap && ((*x+(((int16_t)xo+gw)*textsize_x)) > getWidth())) {
                    *x  = 0; // Reset x to zero, advance y by one line
                    *y += textsize_y * (uint8_t)pgm_read_byte(&gridFont->yAdvance);
                }
                int16_t tsx = (int16_t)textsize_x,
                        tsy = (int16_t)textsize_y,
                        x1 = *x + xo * tsx,
                        y1 = *y + yo * tsy,
                        x2 = x1 + gw * tsx - 1,
                        y2 = y1 + gh * tsy - 1;
                if(x1 < *minx) *minx = x1;
                if(y1 < *miny) *miny = y1;
                if(x2 > *maxx) *maxx = x2;
                if(y2 > *maxy) *maxy = y2;
                *x += xa * tsx;
            }
        }

    } else { // Default font

        if(c == '\n') {                     // Newline?
            *x  = 0;                        // Reset x to zero,
            *y += textsize_y * 8;           // advance y one line
            // min/max x/y unchaged -- that waits for next 'normal' character
        } else if(c != '\r') {  // Normal char; ignore carriage returns
            if(wrap && ((*x + textsize_x * 6) > getWidth())) { // Off right?
                *x  = 0;                    // Reset x to zero,
                *y += textsize_y * 8;       // advance y one line
            }
            int x2 = *x + textsize_x * 6 - 1, // Lower-right pixel of char
                y2 = *y + textsize_y * 8 - 1;
            if(x2 > *maxx) *maxx = x2;      // Track max x, y
            if(y2 > *maxy) *maxy = y2;
            if(*x < *minx) *minx = *x;      // Track min x, y
            if(*y < *miny) *miny = *y;
            *x += textsize_x * 6;             // Advance x one char
        }
    }
}

/**************************************************************************/
/*!
    @brief    Helper to determine size of a string with current font/size. Pass string and a cursor position, returns UL corner and W,H.
    @param    str     The ascii string to measure
    @param    x       The current cursor X
    @param    y       The current cursor Y
    @param    x1      The boundary X coordinate, set by function
    @param    y1      The boundary Y coordinate, set by function
    @param    w      The boundary width, set by function
    @param    h      The boundary height, set by function
*/
/**************************************************************************/
void GridTile::getTextBounds(const char *str, int16_t x, int16_t y,
        int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h) {
    uint8_t c; // Current character

    *x1 = x;
    *y1 = y;
    *w  = *h = 0;

    int16_t minx = getWidth(), miny = getHeight(), maxx = -1, maxy = -1;

    while((c = *str++))
        charBounds(c, &x, &y, &minx, &miny, &maxx, &maxy);

    if(maxx >= minx) {
        *x1 = minx;
        *w  = maxx - minx + 1;
    }
    if(maxy >= miny) {
        *y1 = miny;
        *h  = maxy - miny + 1;
    }
}

/**************************************************************************/
/*!
    @brief    Helper to determine size of a string with current font/size. Pass string and a cursor position, returns UL corner and W,H.
    @param    str    The ascii string to measure (as an arduino String() class)
    @param    x      The current cursor X
    @param    y      The current cursor Y
    @param    x1     The boundary X coordinate, set by function
    @param    y1     The boundary Y coordinate, set by function
    @param    w      The boundary width, set by function
    @param    h      The boundary height, set by function
*/
/**************************************************************************/
void GridTile::getTextBounds(const std::string &str, int16_t x, int16_t y,
        int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h) {
    if (str.length() != 0) {
        getTextBounds(const_cast<char*>(str.c_str()), x, y, x1, y1, w, h);
    }
}


/**************************************************************************/
/*!
    @brief    Helper to determine size of a PROGMEM string with current font/size. Pass string and a cursor position, returns UL corner and W,H.
    @param    str     The flash-memory ascii string to measure
    @param    x       The current cursor X
    @param    y       The current cursor Y
    @param    x1      The boundary X coordinate, set by function
    @param    y1      The boundary Y coordinate, set by function
    @param    w      The boundary width, set by function
    @param    h      The boundary height, set by function
*/
/**************************************************************************/
void GridTile::getTextBounds(std::string *str,
        int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h) {
    uint8_t *s = (uint8_t *)str, c;

    *x1 = x;
    *y1 = y;
    *w  = *h = 0;

    int16_t minx = getWidth(), miny = getHeight(), maxx = -1, maxy = -1;

    while((c = pgm_read_byte(s++)))
        charBounds(c, &x, &y, &minx, &miny, &maxx, &maxy);

    if(maxx >= minx) {
        *x1 = minx;
        *w  = maxx - minx + 1;
    }
    if(maxy >= miny) {
        *y1 = miny;
        *h  = maxy - miny + 1;
    }
}

void GridTile::writeBitmap(CRGBW *data, uint16_t xsize, uint16_t ysize)
{
    for (uint16_t y = 0; y < ysize; y++)
    {
        for (uint16_t x = 0; x < xsize; x++)
        {
            writePixel(x,y,*data);
            data++;
        }
    }
}

uint16_t read16(FILE* f)
{
  // BMP data is stored little-endian, same as Arduino.
  uint16_t result;
  ((uint8_t *)&result)[0] = fgetc(f); // LSB
  ((uint8_t *)&result)[1] = fgetc(f); // MSB
  return result;
}

uint32_t read32(FILE* f)
{
  // BMP data is stored little-endian, same as Arduino.
  uint32_t result;
  ((uint8_t *)&result)[0] = fgetc(f); // LSB
  ((uint8_t *)&result)[1] = fgetc(f);
  ((uint8_t *)&result)[2] = fgetc(f);
  ((uint8_t *)&result)[3] = fgetc(f); // MSB
  return result;
}

#define SD_BUFFER_PIXELS 23

void GridTile::writeBitmapFromSPIFFS(const char *filename, uint8_t x, uint8_t y)
{
    FILE* file;
    uint8_t buffer[3*SD_BUFFER_PIXELS]; // pixel buffer, size for r,g,b
    bool valid = false; // valid format to be handled
    bool flip = true; // bitmap is stored bottom-to-top
    uint32_t pos = 0;
    file =fopen(filename, "r");
    if (file == NULL)
    {
        ESP_LOGE(TAG, "Failed to open file %s", filename);
        return;
    }
    // Parse BMP header
    if (read16(file) == 0x4D42) // BMP signature
    {
        uint32_t fileSize = read32(file);
        uint32_t creatorBytes = read32(file);
        uint32_t imageOffset = read32(file); // Start of image data
        uint32_t headerSize = read32(file);
        uint32_t width  = read32(file);
        uint32_t height = read32(file);
        uint16_t planes = read16(file);
        uint16_t depth = read16(file); // bits per pixel
        uint32_t format = read32(file);
        if ((planes == 1) && (format == 0)) // uncompressed is handled
        {
            ESP_LOGI(TAG, "File %s:", filename);
            ESP_LOGI(TAG, "File Size %i", fileSize);
            ESP_LOGI(TAG, "Image Offset: %i",imageOffset); 
            ESP_LOGI(TAG, "Header size: %i",headerSize); 
            ESP_LOGI(TAG, "Bit Depth: %i",depth); 
            ESP_LOGI(TAG, "Image size: %ix%i",width, height);
            uint32_t rowSize = (width * depth / 8 + 3) & ~3;
            if (height < 0)
            {
                height = -height;
                flip = false;
            }
            uint16_t w = width;
            uint16_t h = height;
            size_t buffidx = sizeof(buffer); // force buffer load
            for (uint16_t row = 0; row < h; row++) // for each line
            {
                if (flip) // Bitmap is stored bottom-to-top order (normal BMP)
                    pos = imageOffset + (height - 1 - row) * rowSize;
                else     // Bitmap is stored top-to-bottom
                    pos = imageOffset + row * rowSize;
                fpos_t filepos;
                fgetpos(file, &filepos);
                if (filepos != pos)
                { // Need seek?
                    fseek(file, pos, SEEK_SET); // if mode is SeekSet, position is set to offset bytes from the beginning.
                    buffidx = sizeof(buffer);   // force buffer reload
                }
                uint8_t bits = 0;
                for (uint16_t col = 0; col < w; col++) // for each pixel
                {
                    // Time to read more pixel data?
                    if (buffidx >= sizeof(buffer))
                    {
                        fread(buffer, sizeof(buffer), 1, file);
                        buffidx = 0; // Set index to beginning
                    }
                    switch (depth)
                    {
                    case 1: // one bit per pixel b/w format
                        {
                            valid = true;
                            if (0 == col % 8)
                            {
                                bits = buffer[buffidx++];
                            }
                            uint16_t bw_color = bits & 0x80;
                            writePixel(col, row, CRGB(bw_color,bw_color,bw_color));
                            bits <<= 1;
                        }
                        break;
                    case 24: // standard BMP format
                        {
                            valid = true;
                            uint16_t b = buffer[buffidx++];
                            uint16_t g = buffer[buffidx++];
                            uint16_t r = buffer[buffidx++];
                            writePixel(col, row, CRGB(r,g,b));
                        }
                        break;
                    }
                } // end pixel
            } // end line
        }
    }
    fclose(file);
    if (valid == false)
    {
        ESP_LOGE(TAG, "File %s not valid", filename);
    }
}

void GridTile::writePNGFromSPIFFS(const char *filename, uint8_t x, uint8_t y)
{
    upng_t* upng;
    uint16_t bufidx = 0;

    upng = upng_new_from_file(filename);
    if (upng != NULL) {
        upng_decode(upng);
        uint16_t fwidth = upng_get_width(upng);
        uint16_t fheight = upng_get_height(upng);
        uint16_t fdepth = upng_get_bitdepth(upng);
        uint16_t fsize = upng_get_size(upng);
        const unsigned char *buf = upng_get_buffer(upng);
        bufidx = 0;
        ESP_LOGI(TAG, "File %s:", filename);
        ESP_LOGI(TAG, "File Size %i", fsize);
        ESP_LOGI(TAG, "Bit Depth: %i",fdepth); 
        ESP_LOGI(TAG, "Image size: %ix%i",fwidth, fheight);
        ESP_LOGI(TAG, "Image Format: %i",upng_get_format(upng)); 
        if (upng_get_error(upng) == UPNG_EOK) {
            switch (upng_get_format(upng))
            {
            case UPNG_RGB8:
                for(uint16_t x=0; x < fwidth; x++){
                    for(uint16_t y=0; y < fheight; y++){
                        CRGB col;
                        col.r = buf[bufidx++];
                        col.g = buf[bufidx++];
                        col.b = buf[bufidx++];
                        writePixel(x,y,col);
                        ESP_LOGI(TAG, "%ix%i:%ir%ig%ib", x,y,col.r,col.g,col.b);
                    }
                }
                break;
            case UPNG_RGBA8:
                for(uint16_t x=0; x < fwidth; x++){
                    for(uint16_t y=0; y < fheight; y++){
                        CRGB col;
                        col.r = buf[bufidx++];
                        col.g = buf[bufidx++];
                        col.b = buf[bufidx++];
                        writePixel(x,y,col);
                        ESP_LOGI(TAG, "%ix%i:%ir%ig%ib", x,y,col.r,col.g,col.b);
                    }
                }
                break;
            case UPNG_LUMINANCE8:
                /* code */
                break;
            
            default:
                break;
            }
        }

        upng_free(upng);
    }
}