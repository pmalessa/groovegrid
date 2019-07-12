/*
 * RandomPixelAnimation.cpp
 *
 *  Created on: 01.04.2019
 *      Author: pmale
 */

#include "RandomPixelAnimation.h"

#include "../driver/Timer.h"

RandomPixelAnimation::RandomPixelAnimation(GridTile* gridTile):GrooveAnimation(gridTile)
{
	animationSpeed = 20;
}

RandomPixelAnimation::~RandomPixelAnimation()
{

}

GrooveApp* RandomPixelAnimation::new_instance(GridTile *tile)
{
	return new RandomPixelAnimation(tile);
}

void RandomPixelAnimation::run()
{
	static uint32_t wait = 0;
	if(frameTimer.isTimeUp())
	{
		if(wait == 0)
		{
			if(esp_random()%2)	//clear or draw
			{
				tile->writePixel(esp_random()%GRID_WIDTH, esp_random()%GRID_HEIGHT, CRGB(esp_random()%256, esp_random()%256, esp_random()%256));
			}
			else
			{
				tile->writePixel(esp_random()%GRID_WIDTH, esp_random()%GRID_HEIGHT, CRGB(0, 0, 0));
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
