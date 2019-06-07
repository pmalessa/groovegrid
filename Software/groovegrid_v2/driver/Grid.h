/*
 * LED.h
 *
 *  Created on: 04.01.2019
 *      Author: pmale
 */

#ifndef GRID_H_
#define GRID_H_

#include "../PLATFORM.h"
#include <FastLED.h>
#include "gamma.h"

class Grid {
 public:
	static Grid& getInstance();
	~Grid(void);

	void         display(void);
	void         clearDisplay(void);
	void 		 writePixel(int16_t x, int16_t y, uint16_t color);
	void 		 drawPixel(int16_t x, int16_t y, uint16_t color);
	void 		 endWrite(void);
	void		 setBrightness(uint8_t brightness);	//set global brightness value 0..255

uint16_t RGB(uint8_t r, uint8_t g, uint8_t b);
uint16_t RGB(uint32_t rgb);
uint16_t HSV(uint8_t h, uint8_t s, uint8_t v);
uint32_t expandColor(uint16_t color);

 private:
	Grid();
	Grid(const Grid&);
	Grid & operator = (const Grid &);
};
#endif /* GRID_H_ */
