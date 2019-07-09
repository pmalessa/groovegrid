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

	void 		 writePixel(int16_t x, int16_t y, CRGB color);
	void 		 endWrite(void);
	void 		 fillScreen(CRGB color);
	void		 fillScreenBuffer(CRGB color);

    void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB color);
    void writeRect(int16_t x, int16_t y, int16_t w, int16_t h, CRGB color);
    void writeFilledRect(int16_t x, int16_t y, int16_t w, int16_t h, CRGB color);

	uint8_t getWidth();
	uint8_t getHeight();
protected:
	uint16_t x0,y0,x1,y1;
private:
    void writeFastVLine(int16_t x, int16_t y, int16_t h, CRGB color);
    void writeFastHLine(int16_t x, int16_t y, int16_t w, CRGB color);
};



#endif /* DRIVER_GRIDTILE_H_ */
