/*
 * ANIMATION.cpp
 *
 *  Created on: 22.09.2017
 *      Author: pmale
 */
#include "LED.h"
#include"ANIMATION.h"

void ANIMATION_vBoot()
{
	for (uint8_t i=0; i < YMAX; i++) {
		LED_vDrawLine(0, i, XMAX-1, i, LED_u16Color(255, 0, 0));
		LED_vShow();
		delay(100);
	}
	for (uint8_t i=0; i < YMAX; i++) {
		LED_vDrawLine(0, i, XMAX-1, i, LED_u16Color(0, 0, 0));
		LED_vShow();
		delay(100);
	}
}

void ANIMATION_randomPixels()
{
	static uint32_t wait = 0;

	if(wait == 0)
	{
		if(rand()%2)	//clear or draw
		{
			LED_vDrawPixel(rand()%4, rand()%4, LED_u16Color(rand()%256, rand()%256, rand()%256));
		}
		else
		{
			LED_vDrawPixel(rand()%4, rand()%4, LED_u16Color(0, 0, 0));
		}

		wait = rand()%300;
	}
	else
	{
		wait--;
	}
}

void ANIMATION_randomLines()
{
	static uint32_t wait = 0;

	if(wait == 0)
	{
		if(rand()%2)
		{
			LED_vDrawLine(rand()%4,rand()%4,rand()%4,rand()%4,LED_u16Color(rand()%256, rand()%256, rand()%256));
		}
		else
		{
			LED_vDrawLine(rand()%4,rand()%4,rand()%4,rand()%4,LED_u16Color(0, 0, 0));
		}

		wait = rand()%300;
	}
	else
	{
		wait--;
	}
}

void ANIMATION_vRunner()
{
	static uint32_t wait = 30000;
	static uint8_t state = 0;
	if(wait == 0)
	{
		state++;	//switch Animation every 10 seconds
		LED_vClear();
		wait = 30000;
	}
	wait--;
	switch (state) {
		case 0:
			ANIMATION_randomPixels();
			break;
		case 1:
			ANIMATION_randomLines();
			break;
		default:
			state = 0;
			break;
	}
	LED_vShow();
}
