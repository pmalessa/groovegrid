/*
 * LED.c
 *
 *  Created on: 04.01.2019
 *      Author: pmale
 */
#include "Grid.h"

#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

//4x4 display, starting at top left and going down zigzag
Adafruit_NeoMatrix Grid::matrix = Adafruit_NeoMatrix(XMAX, YMAX, DATA_PIN,
		  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
		  NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
		  NEO_GRB            + NEO_KHZ800);

Grid& Grid::getInstance()
{
	static Grid _instance;
	return _instance;
}

Grid::~Grid(){}
Grid::Grid()
{
	matrix.begin();
}

void Grid::writePixel(int16_t x, int16_t y, uint16_t color)
{
	matrix.drawPixel(x, y, color);
}

void Grid::drawPixel(int16_t x, int16_t y, uint16_t color)
{
	matrix.drawPixel(x, y, color);
	matrix.show();
}

void Grid::endWrite()
{
	matrix.show();
}

void Grid::clearDisplay()
{
	matrix.clear();
}

uint16_t Grid::RGB(uint8_t r, uint8_t g, uint8_t b)
{
	return matrix.Color(r, g, b);
}
uint16_t Grid::RGB(uint32_t rgb)
{
	return matrix.Color((rgb&0xFF0000)>>16,(rgb&0x00FF00)>>8,(rgb&0x0000FF)>>0);
}

uint16_t Grid::HSV(uint8_t h, uint8_t s, uint8_t v)
{
	//TODO
	return 0;
}
