/*
 * LED.c
 *
 *  Created on: 04.01.2019
 *      Author: pmale
 */
#include "Arduino.h"

#include "LED.h"
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

//4x4 display, starting at top left and going down zigzag
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(XMAX, YMAX, DATA_PIN,
		  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
		  NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
		  NEO_GRB            + NEO_KHZ800);

void LED_vInit()
{
	matrix.begin();
}

void LED_vDrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
	matrix.drawPixel(x, y, color);
}

void LED_vDrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
	matrix.drawRect(x, y, w, h, color);
}

void LED_vDrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
	matrix.drawLine(x0, y0, x1, y1, color);
}

void LED_vClear()
{
	matrix.clear();
}

void LED_vShow()
{
	matrix.show();
}

uint16_t LED_u16Color(uint8_t r, uint8_t g, uint8_t b)
{
	return matrix.Color(r, g, b);
}
