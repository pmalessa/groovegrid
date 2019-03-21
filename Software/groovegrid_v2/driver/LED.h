/*
 * LED.h
 *
 *  Created on: 04.01.2019
 *      Author: pmale
 */

#ifndef GRID_H_
#define GRID_H_

#include "../PLATFORM.h"
#include "Adafruit_GFX.h"

typedef enum{
	COLOR_RED = 0xFF0000,
	COLOR_ORANGE = 0xFF8000,
	COLOR_YELLOW = 0xFFFF00,
	COLOR_LIGHTGREEN = 0x80FF00,
	COLOR_GREEN = 0x00FF00,
	COLOR_BLUEGREEN = 0x00FF80,
	COLOR_CYAN = 0x00FFFF,
	COLOR_LIGHTBLUE = 0x0080FF,
	COLOR_BLUE = 0x0000FF,
	COLOR_VIOLET = 0x8000FF,
	COLOR_PINK = 0xFF00FF,
	COLOR_PINKRED = 0xFF0080,
	COLOR_BLACK = 0x000000,
	COLOR_WHITE = 0xFFFFFF
}color_t;

class Grid : public Adafruit_GFX {
 public:
	static Grid& getInstance();
	~Grid(void);

	void         display(void);
	void         clearDisplay(void);
	void 		 drawPixel(int16_t x, int16_t y, uint16_t color);
	boolean      getPixel(int16_t x, int16_t y);
	void 		 endWrite(void);
	void		 clear(void);

	static uint16_t RGB(uint8_t r, uint8_t g, uint8_t b);
	static uint16_t HSV(uint8_t h, uint8_t s, uint8_t v);

 private:
	Grid();
	Grid(const Grid&);
	Grid & operator = (const Grid &);
	int8_t       data_pin;
	Adafruit_NeoMatrix matrix;
};
#endif /* GRID_H_ */
