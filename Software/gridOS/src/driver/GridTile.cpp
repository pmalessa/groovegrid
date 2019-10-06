/*
 * GridTile.cpp
 *
 *  Created on: 28.03.2019
 *      Author: pmale
 */
#include "GridTile.h"

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif

GridTile::GridTile(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
	this->x0 = x0;
	this->y0 = y0;
	this->x1 = x1;
	this->y1 = y1;
}

uint8_t GridTile::getWidth()
{
	return (x1-x0)+1;
}

uint8_t GridTile::getHeight()
{
	return (y1-y0)+1;
}

void GridTile::writePixel(int16_t x, int16_t y, CRGB color)
{
	static Grid& grid = Grid::getInstance();
	if(!((uint16_t)x < x0 || (uint16_t)y < y0 || (uint16_t)x > x1 || (uint16_t)y > y1))	//if draw is inside tile dimensions
	{
		grid.writePixel(x, y, color);
	}
}

void GridTile::fillScreen(CRGB color)
{
	fillScreenBuffer(color);
	endWrite();
}

void GridTile::fillScreenBuffer(CRGB color) {
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
	//static Grid& grid = Grid::getInstance();
	//grid.endWrite();
}

void GridTile::writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB color)
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

void GridTile::writeRect(int16_t x, int16_t y, int16_t w, int16_t h, CRGB color)
{
    writeFastHLine(x, y, w, color);
    writeFastHLine(x, y+h-1, w, color);
    writeFastVLine(x, y, h, color);
    writeFastVLine(x+w-1, y, h, color);
}

void GridTile::writeFilledRect(int16_t x, int16_t y, int16_t w, int16_t h, CRGB color)
{
    for (int16_t i=x; i<x+w; i++) {
        writeFastVLine(i, y, h, color);
    }
}

void GridTile::writeFastVLine(int16_t x, int16_t y, int16_t h, CRGB color)
{
	writeLine(x, y, x, y+h-1, color);
}

void GridTile::writeFastHLine(int16_t x, int16_t y, int16_t w, CRGB color)
{
	writeLine(x, y, x+w-1, y, color);
}
