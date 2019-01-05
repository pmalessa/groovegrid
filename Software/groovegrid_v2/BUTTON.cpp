/*
 * BUTTON.cpp
 *
 *  Created on: 05.01.2019
 *      Author: pmale
 */

#include "BUTTON.h"
#include <Button.h>


Button bUp = Button(BUTTON_UP);
Button bDown = Button(BUTTON_DOWN);
Button bLeft = Button(BUTTON_LEFT);
Button bRight = Button(BUTTON_RIGHT);

void BUTTON_vInit()
{
	bUp.begin();
	bDown.begin();
	bLeft.begin();
	bRight.begin();
}

void BUTTON_vRead()
{
	bUp.read();
	bDown.read();
	bLeft.read();
	bRight.read();
}

bool BUTTON_bIsPressed(uint8_t id)
{
	switch (id) {
		case BUTTON_UP:
			return bUp.pressed();
			break;
		case BUTTON_LEFT:
			return bLeft.pressed();
			break;
		case BUTTON_RIGHT:
			return bRight.pressed();
			break;
		case BUTTON_DOWN:
			return bDown.pressed();
			break;
		default:
			return false;
			break;
	}
}
