/*
 * LED.h
 *
 *  Created on: 04.01.2019
 *      Author: pmale
 */

#ifndef GRID_H_
#define GRID_H_

#include "../PLATFORM.h"
#include "FastLED.h"
#include "FastLED_RGBW.h"
#include "gamma.h"
#include "driver/Storage.h"

class Grid {
 public:
	static void init();
	static void display(void);
	static void clearDisplay(void);
	static void writePixel(int16_t x, int16_t y, CRGBW color);
	static void endWrite(void);
	static void	setGlobalBrightness(uint8_t brightness);	//set global brightness value 0..255

 private:
	static bool isInitialized;
};
#endif /* GRID_H_ */
