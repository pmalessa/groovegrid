/*
 * GridTile.cpp
 *
 *  Created on: 28.03.2019
 *      Author: pmale
 */
#include "GridTile.h"

GridTile::GridTile(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1):Adafruit_GFX(x1-x0+1, y1-y0+1)
{
	this->x0 = x0;
	this->y0 = y0;
	this->x1 = x1;
	this->y1 = y1;
	this->TileHeight = (y1-y0)+1;
	this->TileWidth = (x1-x0)+1;
	this->pixelMultiplier = 1;
}

uint8_t GridTile::getWidth()
{
	return this->TileWidth;
}

uint8_t GridTile::getHeight()
{
	return this->TileHeight;
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

void GridTile::fillScreen(uint16_t color)
{
	fillScreenBuffer(color);
	endWrite();
}

void GridTile::fillScreenBuffer(uint16_t color) {
	for(uint16_t x = x0; x <= x1; x++)
		{
			for(uint16_t y = y0; y <= y1; y++)
			{
				writePixel(x, y, color);
			}
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
