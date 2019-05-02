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

GrooveApp* RandomLineAnimation::new_instance(GridTile *tile)
{
	return new RandomLineAnimation(tile);
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
				tile->drawLine(esp_random()%GRID_WIDTH, esp_random()%GRID_HEIGHT, esp_random()%GRID_WIDTH, esp_random()%GRID_HEIGHT, tile->RGB(esp_random()%256, esp_random()%256, esp_random()%256));
			}
			else
			{
				tile->drawLine(esp_random()%GRID_WIDTH, esp_random()%GRID_HEIGHT, esp_random()%GRID_WIDTH, esp_random()%GRID_HEIGHT, tile->RGB(0, 0, 0));
			}

			wait = esp_random()%animationSpeed;
		}
		else
		{
			wait--;
		}
	}
}
