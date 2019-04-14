/*
 * GridTile.cpp
 *
 *  Created on: 28.03.2019
 *      Author: pmale
 */
#include "GridTile.h"

GridTile::GridTile(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1):Adafruit_GFX(x1-x0, y1-y0)
{
	GridTile::x0 = x0;
	GridTile::y0 = y0;
	GridTile::x1 = x1;
	GridTile::y1 = y1;
	GridTile::pixelMultiplier = 1;
}

void GridTile::writePixel(int16_t x, int16_t y, uint16_t color)
{
	static Grid& grid = Grid::getInstance();
	if(!((uint16_t)x < x0 || (uint16_t)y < y0 || (uint16_t)x > x1 || (uint16_t)y > y1))	//if draw is inside tile dimensions
	{
		grid.writePixel(x, y, color);
	}
}
void GridTile::drawPixel(int16_t x, int16_t y, uint16_t color)
{
	static Grid& grid = Grid::getInstance();
	if(!((uint16_t)x < x0 || (uint16_t)y < y0 || (uint16_t)x > x1 || (uint16_t)y > y1))	//if draw is inside tile dimensions
	{
		grid.drawPixel(x, y, color);
	}
}

void GridTile::endWrite()
{
	static Grid& grid = Grid::getInstance();
	grid.endWrite();
}

uint16_t GridTile::RGB(uint8_t r, uint8_t g, uint8_t b)
{
	static Grid& grid = Grid::getInstance();
	return grid.RGB(r,g,b);
}
uint16_t GridTile::RGB(uint32_t rgb)
{
	static Grid& grid = Grid::getInstance();
	return grid.RGB(rgb);
}
uint16_t GridTile::HSV(uint8_t h, uint8_t s, uint8_t v)
{
	static Grid& grid = Grid::getInstance();
	return grid.HSV(h, s, v);
}
