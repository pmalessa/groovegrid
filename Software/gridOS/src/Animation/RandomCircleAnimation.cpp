/*
 * RandomCircleAnimation.cpp
 *
 *  Created on: 01.04.2019
 *      Author: pmale
 */

#include "RandomCircleAnimation.h"

#include "../driver/Timer.h"

RandomCircleAnimation::RandomCircleAnimation(GridTile* gridTile):GrooveAnimation(gridTile)
{
	animationSpeed = 8;
}

RandomCircleAnimation::~RandomCircleAnimation()
{

}

void RandomCircleAnimation::run()
{
	static uint32_t wait = 0;
	if(frameTimer.isTimeUp())
	{
		if(wait == 0)
		{
			if(esp_random()%2)	//clear or draw
			{
				tile->writeFilledCircle(esp_random()%GRID_WIDTH,esp_random()%GRID_HEIGHT,esp_random()%(GRID_HEIGHT/2),CRGBW(esp_random()%256, esp_random()%256, esp_random()%256, 0));
			}
			else
			{
				tile->writeFilledCircle(esp_random()%GRID_WIDTH,esp_random()%GRID_HEIGHT,esp_random()%(GRID_HEIGHT/2),CRGBW(0,0,0, 0));
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
