/*
 * LED.c
 *
 *  Created on: 04.01.2019
 *      Author: pmale
 */
#include "Grid.h"

#ifdef DOOR16
	CRGB matrixleds[GRID_WIDTH*GRID_HEIGHT];
#elif defined(ROVER)
	CRGB matrixleds[GRID_WIDTH*GRID_HEIGHT];
	//FastLED.setMaxRefreshRate(100, 0);
#elif defined(TABLE)
#ifdef STRIP_RGBW
	CRGBW matrixleds[NUM_STRIPS][NUM_LEDS_PER_CHANNEL];	// FastLED with RGBW
#else
	CRGB matrixleds[NUM_STRIPS][NUM_LEDS_PER_CHANNEL];
#endif
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
	FastLED.setBrightness(100);
	//FastLED.setMaxRefreshRate(100, 0);
#elif defined(TABLE)
#ifdef STRIP_RGBW
	uint16_t rgbw_size = getRGBWsize(NUM_LEDS_PER_CHANNEL);
	FastLED.addLeds<WS2812B,GRID_DATA1_PIN>((CRGB *)&matrixleds[0][0], 0, rgbw_size);
	FastLED.addLeds<WS2812B,GRID_DATA2_PIN>((CRGB *)&matrixleds[1][0], 0, rgbw_size);
	FastLED.addLeds<WS2812B,GRID_DATA3_PIN>((CRGB *)&matrixleds[2][0], 0, rgbw_size);
	FastLED.addLeds<WS2812B,GRID_DATA4_PIN>((CRGB *)&matrixleds[3][0], 0, rgbw_size);
	FastLED.addLeds<WS2812B,GRID_DATA5_PIN>((CRGB *)&matrixleds[4][0], 0, rgbw_size);
	FastLED.addLeds<WS2812B,GRID_DATA6_PIN>((CRGB *)&matrixleds[5][0], 0, rgbw_size);
	FastLED.addLeds<WS2812B,GRID_DATA7_PIN>((CRGB *)&matrixleds[6][0], 0, rgbw_size);
	FastLED.addLeds<WS2812B,GRID_DATA8_PIN>((CRGB *)&matrixleds[7][0], 0, rgbw_size/2);	//only 1 strip on Pin8
#else
	FastLED.addLeds<WS2812B,GRID_DATA1_PIN>(&matrixleds[0][0], 0, NUM_LEDS_PER_CHANNEL);
	FastLED.addLeds<WS2812B,GRID_DATA2_PIN>(&matrixleds[1][0], 0, NUM_LEDS_PER_CHANNEL);
	FastLED.addLeds<WS2812B,GRID_DATA3_PIN>(&matrixleds[2][0], 0, NUM_LEDS_PER_CHANNEL);
	FastLED.addLeds<WS2812B,GRID_DATA4_PIN>(&matrixleds[3][0], 0, NUM_LEDS_PER_CHANNEL);
	FastLED.addLeds<WS2812B,GRID_DATA5_PIN>(&matrixleds[4][0], 0, NUM_LEDS_PER_CHANNEL);
	FastLED.addLeds<WS2812B,GRID_DATA6_PIN>(&matrixleds[5][0], 0, NUM_LEDS_PER_CHANNEL);
	FastLED.addLeds<WS2812B,GRID_DATA7_PIN>(&matrixleds[6][0], 0, NUM_LEDS_PER_CHANNEL);
	FastLED.addLeds<WS2812B,GRID_DATA8_PIN>(&matrixleds[7][0], 0, NUM_LEDS_PER_CHANNEL/2);	//only 1 strip on Pin8
#endif
	FastLED.setBrightness(100);
#endif
}

void Grid::writePixel(int16_t x, int16_t y, CRGBW color)
{
#ifdef STRIP_RGBW
	CRGBW convColor;
	convCol = color;
#else
	CRGB convColor;
	convColor.r = color.r;
	convColor.g = color.g;
	convColor.b = color.b;
#endif
    if(!(x < 0 || y < 0 || x >= GRID_WIDTH || y >= GRID_HEIGHT))	//if draw is inside grid dimensions
	{
		if((y&0x01) == 0)	//even row
		{
	#ifdef DOOR16	//First LED Top Left
			matrixleds[GRID_WIDTH*y + x] = convColor;
	#elif defined(ROVER)			//First LED Top Right
			matrixleds[GRID_WIDTH*y + (GRID_WIDTH-x)-1] = convColor;
	#elif defined(TABLE)
			matrixleds[y>>1][GRID_WIDTH*(y&0x01)+(GRID_WIDTH-x)-1] = convColor;
	#endif
		}
		else			//odd row
		{
	#ifdef DOOR16	//First LED Top Left
			matrixleds[GRID_WIDTH*y + (GRID_WIDTH-x)-1] = convColor;
	#elif defined(ROVER)			//First LED Top Right
			matrixleds[GRID_WIDTH*y + x] = convColor;
	#elif defined(TABLE)
			matrixleds[y>>1][GRID_WIDTH*(y&0x01)+x] = convColor;
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
