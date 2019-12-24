/*
 * GridTile.h
 *
 *  Created on: 28.03.2019
 *      Author: pmale
 */

#ifndef DRIVER_GRIDTILE_H_
#define DRIVER_GRIDTILE_H_

#include "../PLATFORM.h"
#include "Grid.h"
#include "GridFont.h"
//An object of GridTile gets passed to each App to draw on it.
//it has its own dimensions, and the App can only draw on this tile of the Grid
class GridTile
{
public:
	//Specify the size by giving it the start and end points in the grid
	GridTile(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

	void 		 writePixel(int16_t x, int16_t y, CRGB color);
	void 		 endWrite(void);
	void 		 fillScreen(CRGB color);
	void		 fillScreenBuffer(CRGB color);

    void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB color);
    void writeRect(int16_t x, int16_t y, int16_t w, int16_t h, CRGB color);
    void writeFilledRect(int16_t x, int16_t y, int16_t w, int16_t h, CRGB color);
	void writeCircle(int16_t x0, int16_t y0, int16_t r,CRGB color);
	void writeFilledCircle(int16_t x0, int16_t y0, int16_t r, CRGB color);
	void writeString(int16_t x, int16_t y, std::string str, CRGB color, CRGB bg, uint8_t size);
	void writeChar(int16_t x, int16_t y, unsigned char c, CRGB color, CRGB bg, uint8_t size);
	void writeChar(int16_t x, int16_t y, unsigned char c, CRGB color, CRGB bg, uint8_t size_x, uint8_t size_y);
	void setTextSize(uint8_t s);
	void setTextSize(uint8_t s_x, uint8_t s_y);
	void setFont(const GridFont *f);
	uint8_t getWidth();
	uint8_t getHeight();
	uint8_t getRotation();
	void setRotation(uint8_t rot);	//rotation by 90 Deg clockwise, set Coordinate Origin at different spot
protected:
	uint16_t x0,y0,x1,y1, cursor_x, cursor_y, rotation;
	uint8_t textsize_x, textsize_y;
	bool wrap, _cp437;
	GridFont *gridFont;
	static Grid &grid;
private:
    void writeFastVLine(int16_t x, int16_t y, int16_t h, CRGB color);
    void writeFastHLine(int16_t x, int16_t y, int16_t w, CRGB color);
	void drawCircleHelper( int16_t x0, int16_t y0,int16_t r, uint8_t cornername, CRGB color);
	void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t corners, int16_t delta, CRGB color);
	void getTextBounds(std::string *str,int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
	void getTextBounds(const std::string &str, int16_t x, int16_t y,int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
	void getTextBounds(const char *str, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
	void charBounds(char c, int16_t *x, int16_t *y, int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy);
};



#endif /* DRIVER_GRIDTILE_H_ */
