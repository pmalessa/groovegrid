/*
 * LED.c
 *
 *  Created on: 04.01.2019
 *      Author: pmale
 */
#include "Grid.h"

CRGB matrixleds[GRID_WIDTH*GRID_HEIGHT];

Grid& Grid::getInstance()
{
	static Grid _instance;
	return _instance;
}

Grid::~Grid(){}
Grid::Grid()
{
#ifdef DOOR16
	FastLED.addLeds<NEOPIXEL,GRID_DATA_PIN>(matrixleds, GRID_WIDTH*GRID_HEIGHT);
	FastLED.setBrightness(16);
#else
	FastLED.addLeds<NEOPIXEL,GRID_DATA1_PIN>(matrixleds, NUM_LEDS_PER_CHANNEL*0, NUM_LEDS_PER_CHANNEL);
	FastLED.addLeds<NEOPIXEL,GRID_DATA2_PIN>(matrixleds, NUM_LEDS_PER_CHANNEL*1, NUM_LEDS_PER_CHANNEL);
	FastLED.addLeds<NEOPIXEL,GRID_DATA3_PIN>(matrixleds, NUM_LEDS_PER_CHANNEL*2, NUM_LEDS_PER_CHANNEL);
	FastLED.addLeds<NEOPIXEL,GRID_DATA4_PIN>(matrixleds, NUM_LEDS_PER_CHANNEL*3, NUM_LEDS_PER_CHANNEL);
	FastLED.addLeds<NEOPIXEL,GRID_DATA5_PIN>(matrixleds, NUM_LEDS_PER_CHANNEL*4, NUM_LEDS_PER_CHANNEL);
	FastLED.addLeds<NEOPIXEL,GRID_DATA6_PIN>(matrixleds, NUM_LEDS_PER_CHANNEL*5, NUM_LEDS_PER_CHANNEL);
	FastLED.addLeds<NEOPIXEL,GRID_DATA7_PIN>(matrixleds, NUM_LEDS_PER_CHANNEL*6, NUM_LEDS_PER_CHANNEL);
	FastLED.setBrightness(255);
	//FastLED.setMaxRefreshRate(100, 0);
#endif
}

void Grid::writePixel(int16_t x, int16_t y, CRGB color)
{
	if(y%2 == 0)	//even row
	{
#ifdef DOOR16	//First LED Top Left
		matrixleds[GRID_WIDTH*y + x] = color;
#else			//First LED Top Right
		matrixleds[GRID_WIDTH*y + (GRID_WIDTH-x)-1] = color;
#endif
	}
	else			//odd row
	{
#ifdef DOOR16	//First LED Top Left
		matrixleds[GRID_WIDTH*y + (GRID_WIDTH-x)-1] = color;
#else			//First LED Top Right
		matrixleds[GRID_WIDTH*y + x] = color;
#endif
	}
}

void Grid::endWrite()
{
	FastLED.show();
}

void Grid::clearDisplay()
{
	FastLED.clear(true);
}

void Grid::setBrightness(uint8_t brightness)
{
	FastLED.setBrightness(brightness);
}
