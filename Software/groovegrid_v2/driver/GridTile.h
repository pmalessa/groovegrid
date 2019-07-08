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
//An object of GridTile gets passed to each App to draw on it.
//it has its own dimensions, and the App can only draw on this tile of the Grid
class GridTile
{
public:
	//Specify the size by giving it the start and end points in the grid
	GridTile(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

	//Those functions depend on the dimension, therefore they have to be overridden
	void 		 writePixel(int16_t x, int16_t y, uint16_t color);
	void 		 writePixel(int16_t x, int16_t y, CRGB color);
	void 		 endWrite(void);
	void 		 fillScreen(uint16_t color);
	void		 fillScreenBuffer(uint16_t color);

    void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
    void writeRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void writeFilledRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

	uint8_t getWidth();
	uint8_t getHeight();

	uint16_t RGB(uint8_t r, uint8_t g, uint8_t b);
	uint16_t RGB(uint32_t rgb);
	uint16_t fromCRGB(CRGB crgb);
	uint16_t HSV(uint8_t h, uint8_t s, uint8_t v);
protected:
	uint16_t x0,y0,x1,y1;
private:
    void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
    void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
};



#endif /* DRIVER_GRIDTILE_H_ */
