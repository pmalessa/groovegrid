/*
 * LED.h
 *
 *  Created on: 04.01.2019
 *      Author: pmale
 */

#ifndef LED_H_
#define LED_H_

#include "../PLATFORM.h"

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

void LED_vInit();
void LED_vDrawPixel(uint16_t x, uint16_t y, uint16_t color);
void LED_vDrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void LED_vDrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void LED_vClear();
void LED_vShow();
uint16_t LED_u16Color(uint8_t r, uint8_t g, uint8_t b);
uint16_t LED_u16rgbColor(uint32_t rgb_color);

#endif /* LED_H_ */
