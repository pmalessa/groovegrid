/*
 * LED.c
 *
 *  Created on: 04.01.2019
 *      Author: pmale
 */
#include "Grid.h"

static const char* TAG = "Grid";
bool Grid::isInitialized = false;

#ifdef DOOR16
	CRGB matrixleds[GRID_WIDTH*GRID_HEIGHT];
#elif defined(ROVER)
	CRGB matrixleds[GRID_WIDTH*GRID_HEIGHT];
	//FastLED.setMaxRefreshRate(100, 0);
#elif defined(TABLE)
	CRGBW matrixleds_rgbw[8][NUM_LEDS_PER_CHANNEL];	// FastLED with RGBW
#endif

void Grid::init()
{
	if(!isInitialized){
		isInitialized = true;
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
		FastLED.addLeds<SK6812,GRID_DATA1_PIN>((CRGB *)&matrixleds_rgbw[0][0], 0, rgbw_size);
		FastLED.addLeds<SK6812,GRID_DATA2_PIN>((CRGB *)&matrixleds_rgbw[1][0], 0, rgbw_size);
		FastLED.addLeds<SK6812,GRID_DATA3_PIN>((CRGB *)&matrixleds_rgbw[2][0], 0, rgbw_size);
		FastLED.addLeds<SK6812,GRID_DATA4_PIN>((CRGB *)&matrixleds_rgbw[3][0], 0, rgbw_size);
		FastLED.addLeds<SK6812,GRID_DATA5_PIN>((CRGB *)&matrixleds_rgbw[4][0], 0, rgbw_size);
		FastLED.addLeds<SK6812,GRID_DATA6_PIN>((CRGB *)&matrixleds_rgbw[5][0], 0, rgbw_size);
		FastLED.addLeds<SK6812,GRID_DATA7_PIN>((CRGB *)&matrixleds_rgbw[6][0], 0, rgbw_size);
		FastLED.addLeds<SK6812,GRID_DATA8_PIN>((CRGB *)&matrixleds_rgbw[7][0], 0, rgbw_size/2);	//only 1 strip on Pin8
		uint32_t brightness = Storage::getConfig("Brightness");
		ESP_LOGI(TAG,"Brightness: %i",brightness);
		FastLED.setBrightness(brightness);
#endif
	}
}

void Grid::writePixel(int16_t x, int16_t y, CRGBW color)
{
    if(!(x < 0 || y < 0 || x >= GRID_WIDTH || y >= GRID_HEIGHT))	//if draw is inside grid dimensions
	{
		if((y&0x01) == 0)	//even row
		{
	#ifdef DOOR16	//First LED Top Left
			matrixleds[GRID_WIDTH*y + x] = color;
	#elif defined(ROVER)			//First LED Top Right
			matrixleds[GRID_WIDTH*y + (GRID_WIDTH-x)-1] = color;
	#elif defined(TABLE)
			matrixleds_rgbw[y>>1][GRID_WIDTH*(y&0x01)+(GRID_WIDTH-x)-1] = color;
	#endif
		}
		else			//odd row
		{
	#ifdef DOOR16	//First LED Top Left
			matrixleds[GRID_WIDTH*y + (GRID_WIDTH-x)-1] = color;
	#elif defined(ROVER)			//First LED Top Right
			matrixleds[GRID_WIDTH*y + x] = color;
	#elif defined(TABLE)
			matrixleds_rgbw[y>>1][GRID_WIDTH*(y&0x01)+x] = color;
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

void Grid::setGlobalBrightness(uint8_t brightness)
{
	FastLED.setBrightness(brightness);
	Storage::setConfig(SET_BRIGHTNESS,brightness);
}
