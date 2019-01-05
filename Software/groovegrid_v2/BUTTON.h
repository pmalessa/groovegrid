/*
 * BUTTON.h
 *
 *  Created on: 05.01.2019
 *      Author: pmale
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include "Arduino.h"

#define BUTTON_UP 2
#define BUTTON_DOWN 4
#define BUTTON_LEFT 3
#define BUTTON_RIGHT 5

void BUTTON_vInit();

void BUTTON_vRead();

bool BUTTON_bIsPressed(uint8_t id);




#endif /* BUTTON_H_ */
