/*
 * LED.c
 *
 *  Created on: 04.01.2019
 *      Author: pmale
 */
#include "LED.h"

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

//4x4 display, starting at top left and going down zigzag
Adafruit_NeoMatrix Grid::matrix;

static Grid& Grid::getInstance()
{
	static Grid _instance;
	return _instance;
}

Grid::~Grid(){}
Grid::Grid():Adafruit_GFX(XMAX, YMAX)
{
	data_pin = DATA_PIN;
	uint8_t b = 0;
	matrix = Adafruit_NeoMatrix(_width, _height, data_pin,
			  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
			  NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
			  NEO_GRB            + NEO_KHZ800);
	matrix.begin();
}

void Grid::drawPixel(int16_t x, int16_t y, uint16_t color)
{
	matrix.drawPixel(x, y, color);
}

void Grid::endWrite()
{
	matrix.show();
}

void Grid::clear()
{
	matrix.clear();
}

static uint16_t Grid::RGB(uint8_t r, uint8_t g, uint8_t b)
{
	return matrix.Color(r, g, b);
}
static uint16_t Grid::HSV(uint8_t h, uint8_t s, uint8_t v)
{
	//TODO
}
