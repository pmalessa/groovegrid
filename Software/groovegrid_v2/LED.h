/*
 * LED.h
 *
 *  Created on: 04.01.2019
 *      Author: pmale
 */

#ifndef LED_H_
#define LED_H_

#include "Arduino.h"

#define XMAX 4
#define YMAX 4
#define DATA_PIN 6

void LED_vInit();
void LED_vDrawPixel(uint16_t x, uint16_t y, uint16_t color);
void LED_vDrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void LED_vDrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void LED_vClear();
void LED_vShow();
void LED_vDrawBoard(uint16_t arr[YMAX][XMAX]);
void LED_vDrawTile(uint16_t x, uint16_t y, uint16_t number);
uint16_t LED_u16Color(uint8_t r, uint8_t g, uint8_t b);

#endif /* LED_H_ */
