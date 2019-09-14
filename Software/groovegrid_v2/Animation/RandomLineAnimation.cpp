/*
 * RandomPixelAnimation.cpp
 *
 *  Created on: 01.04.2019
 *      Author: pmale
 */

#include "RandomLineAnimation.h"

RandomLineAnimation::RandomLineAnimation(GridTile* gridTile):GrooveAnimation(gridTile)
{
	animationSpeed = 20;
}

RandomLineAnimation::~RandomLineAnimation()
{

}

void RandomLineAnimation::run()
{
	static uint32_t wait = 0;
	if(frameTimer.isTimeUp())
	{
		if(wait == 0)
		{
			if(esp_random()%2)
			{
				tile->writeLine(esp_random()%GRID_WIDTH, esp_random()%GRID_HEIGHT, esp_random()%GRID_WIDTH, esp_random()%GRID_HEIGHT, CRGB(esp_random()%256, esp_random()%256, esp_random()%256));
			}
			else
			{
				tile->writeLine(esp_random()%GRID_WIDTH, esp_random()%GRID_HEIGHT, esp_random()%GRID_WIDTH, esp_random()%GRID_HEIGHT, CRGB(0, 0, 0));
			}
			tile->endWrite();
			wait = esp_random()%animationSpeed;
		}
		else
		{
			wait--;
		}
	}
}
