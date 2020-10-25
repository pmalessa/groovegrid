/*
 * LED.c
 *
 *  Created on: 04.01.2019
 *      Author: pmale
 */
#include "Grid.h"
#include "FastLED_RGBW.h"

#ifdef DOOR16
	CRGB matrixleds[GRID_WIDTH*GRID_HEIGHT];
#elif defined(ROVER)
	CRGB matrixleds[GRID_WIDTH*GRID_HEIGHT];
	//FastLED.setMaxRefreshRate(100, 0);
#elif defined(TABLE)
	CRGBW matrixleds_rgbw[GRID_WIDTH*GRID_HEIGHT];	// FastLED with RGBW
	CRGB *matrixleds = (CRGB *) &matrixleds_rgbw[0];
#endif

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
#elif defined(ROVER)
	FastLED.addLeds<NEOPIXEL,GRID_DATA1_PIN>(matrixleds, NUM_LEDS_PER_CHANNEL*0, NUM_LEDS_PER_CHANNEL);
	FastLED.addLeds<NEOPIXEL,GRID_DATA2_PIN>(matrixleds, NUM_LEDS_PER_CHANNEL*1, NUM_LEDS_PER_CHANNEL);
	FastLED.addLeds<NEOPIXEL,GRID_DATA3_PIN>(matrixleds, NUM_LEDS_PER_CHANNEL*2, NUM_LEDS_PER_CHANNEL);
	FastLED.addLeds<NEOPIXEL,GRID_DATA4_PIN>(matrixleds, NUM_LEDS_PER_CHANNEL*3, NUM_LEDS_PER_CHANNEL);
	FastLED.addLeds<NEOPIXEL,GRID_DATA5_PIN>(matrixleds, NUM_LEDS_PER_CHANNEL*4, NUM_LEDS_PER_CHANNEL);
	FastLED.addLeds<NEOPIXEL,GRID_DATA6_PIN>(matrixleds, NUM_LEDS_PER_CHANNEL*5, NUM_LEDS_PER_CHANNEL);
	FastLED.addLeds<NEOPIXEL,GRID_DATA7_PIN>(matrixleds, NUM_LEDS_PER_CHANNEL*6, NUM_LEDS_PER_CHANNEL);
	FastLED.setBrightness(255);
	//FastLED.setMaxRefreshRate(100, 0);
#elif defined(TABLE)
	uint16_t rgbw_size = getRGBWsize(NUM_LEDS_PER_CHANNEL);
	FastLED.addLeds<SK6812,GRID_DATA1_PIN>(matrixleds, rgbw_size*0, rgbw_size);
	FastLED.addLeds<SK6812,GRID_DATA2_PIN>(matrixleds, rgbw_size*1, rgbw_size);
	FastLED.addLeds<SK6812,GRID_DATA3_PIN>(matrixleds, rgbw_size*2, rgbw_size);
	FastLED.addLeds<SK6812,GRID_DATA4_PIN>(matrixleds, rgbw_size*3, rgbw_size);
	FastLED.addLeds<SK6812,GRID_DATA5_PIN>(matrixleds, rgbw_size*4, rgbw_size);
	FastLED.addLeds<SK6812,GRID_DATA6_PIN>(matrixleds, rgbw_size*5, rgbw_size);
	FastLED.addLeds<SK6812,GRID_DATA7_PIN>(matrixleds, rgbw_size*6, rgbw_size);
	FastLED.addLeds<SK6812,GRID_DATA8_PIN>(matrixleds, rgbw_size*7, rgbw_size/2);	//only 1 strip on Pin8
	FastLED.setBrightness(255);
#endif
}

void Grid::writePixel(int16_t x, int16_t y, CRGB color)
{
    if(!(x < 0 || y < 0 || x >= GRID_WIDTH || y >= GRID_HEIGHT))	//if draw is inside grid dimensions
	{
		if(y%2 == 0)	//even row
		{
	#ifdef DOOR16	//First LED Top Left
			matrixleds[GRID_WIDTH*y + x] = color;
	#elif defined(ROVER)			//First LED Top Right
			matrixleds[GRID_WIDTH*y + (GRID_WIDTH-x)-1] = color;
	#elif defined(TABLE)
			matrixleds_rgbw[GRID_WIDTH*y + (GRID_WIDTH-x)-1] = color;
	#endif
		}
		else			//odd row
		{
	#ifdef DOOR16	//First LED Top Left
			matrixleds[GRID_WIDTH*y + (GRID_WIDTH-x)-1] = color;
	#elif defined(ROVER)			//First LED Top Right
			matrixleds[GRID_WIDTH*y + x] = color;
	#elif defined(TABLE)
			matrixleds_rgbw[GRID_WIDTH*y + x] = color;
	#endif
		}
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
