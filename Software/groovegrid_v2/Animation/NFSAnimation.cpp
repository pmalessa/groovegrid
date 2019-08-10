/*
 * NFSAnimation.cpp
 *
 *  Created on: 02.08.2019
 *      Author: pmale
 */

#include "NFSAnimation.h"

NFSAnimation::NFSAnimation(GridTile* gridTile):GrooveAnimation(gridTile)
{
	animationSpeed = 20;
	screenBuffer = new uint8_t*[tile->getWidth()];
	for(uint8_t i = 0; i < tile->getWidth(); i++)
	{
		screenBuffer[i] = new uint8_t[tile->getHeight()];
	}
}

NFSAnimation::~NFSAnimation()
{
	for(uint8_t i = 0; i < tile->getWidth(); i++)
	{
		delete screenBuffer[i];
	}
	delete screenBuffer;
}

GrooveApp* NFSAnimation::new_instance(GridTile *tile)
{
	return new NFSAnimation(tile);
}

void NFSAnimation::run()
{
	if(frameTimer.isTimeUp())
	{
		for(uint8_t i=0;i<tile->getHeight();i++)	//generate line
		{
			uint8_t length = esp_random()%10;

			for(uint8_t k=0;k<length;k++)
			{
				for(uint8_t j=0;j<tile->getWidth()-1;j++)
				{
					screenBuffer[j][i] = screenBuffer[j+1][i];	//move pixel down
				}
			}
			screenBuffer[tile->getWidth()-1][i] = esp_random()%5;	//green or black
		}

		for(uint8_t i=0;i<tile->getHeight();i++)	//draw pixel
		{
			for(uint8_t j=0;j<tile->getWidth();j++)
			{
				if(screenBuffer[j][i] == 0)
				{
					tile->writePixel(j, i, CRGB(0,0,255));
				}
				else
				{
					tile->writePixel(j, i, CRGB(0, 0, 0));
				}
			}
		}
	}
}



