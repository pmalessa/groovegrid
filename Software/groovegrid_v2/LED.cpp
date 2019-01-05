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


const uint16_t colors[11] =
	{matrix.Color(255, 0, 0),		//RED
	 matrix.Color(0, 255, 0),		//GREEN
	 matrix.Color(0, 0, 255),		//BLUE
	 matrix.Color(255, 255, 255),	//WHITE
	 matrix.Color(255, 255, 0),		//YELLOW
	 matrix.Color(255, 0, 255),		//VIOLET
	 matrix.Color(0, 255, 255),		//CYAN
	 matrix.Color(150, 100, 30),	//MIX1
	 matrix.Color(30, 150, 100),	//MIX2
	 matrix.Color(100, 30, 150),	//MIX3
	 matrix.Color(230, 100, 100),	//MIX4
	};

void LED_vInit()
{
	matrix.begin();
}

void LED_vDrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
	matrix.drawPixel(x, y, color);
}

void LED_vDrawTile(uint16_t x, uint16_t y, uint16_t number)
{
	uint16_t col = 0;
	switch (number) {
		case 2:
			 col =colors[0];
			break;
		case 4:
			 col =colors[1];
			break;
		case 8:
			 col =colors[2];
			break;
		case 16:
			 col =colors[3];
			break;
		case 32:
			 col =colors[4];
			break;
		case 64:
			 col =colors[5];
			break;
		case 128:
			 col =colors[6];
			break;
		case 256:
			 col =colors[7];
			break;
		case 512:
			 col =colors[8];
			break;
		case 1024:
			 col =colors[9];
			break;
		case 2048:
			 col =colors[10];
			break;
		default:
			break;
	}
	matrix.drawPixel(x, y, col);
}

void LED_vDrawBoard(uint16_t arr[YMAX][XMAX])
{
    for (uint8_t i = 0; i < YMAX; i++)
      for (uint8_t j = 0; j < XMAX; j++)
        LED_vDrawTile(j, i, arr[i][j]);
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
