/*
 * ANIMATION.cpp
 *
 *  Created on: 22.09.2017
 *      Author: pmale
 */
#include"ANIMATION.h"

void ANIMATION_randomPixels(Adafruit_NeoMatrix *matrix)
{
	static uint32_t wait = 0;

	if(wait == 0)
	{
		if(rand()%2)	//clear or draw
		{
			matrix->drawPixel(rand()%4, rand()%4, matrix->Color(rand()%256, rand()%256, rand()%256));
		}
		else
		{
			matrix->drawPixel(rand()%4, rand()%4, matrix->Color(0, 0, 0));
		}

		wait = rand()%300;
	}
	else
	{
		wait--;
	}
}

void ANIMATION_randomLines(Adafruit_NeoMatrix *matrix)
{
	static uint32_t wait = 0;

	if(wait == 0)
	{
		if(rand()%2)
		{
			matrix->drawLine(rand()%4,rand()%4,rand()%4,rand()%4,matrix->Color(rand()%256, rand()%256, rand()%256));
		}
		else
		{
			matrix->drawLine(rand()%4,rand()%4,rand()%4,rand()%4,matrix->Color(0, 0, 0));
		}

		wait = rand()%300;
	}
	else
	{
		wait--;
	}
}
