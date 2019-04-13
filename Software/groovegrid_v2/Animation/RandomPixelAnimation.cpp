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
	animationStartTime = 0;
	animationSpeed = 5;
}

RandomPixelAnimation::~RandomPixelAnimation()
{

}

void RandomPixelAnimation::run()
{
	static uint32_t wait = 0;
	if(Timer::getMillis()-animationStartTime > animationSpeed)
	{
		animationStartTime = Timer::getMillis();
		if(wait == 0)
		{
			if(rand()%2)	//clear or draw
			{
				tile->drawPixel(rand()%XMAX, rand()%YMAX, tile->RGB(rand()%256, rand()%256, rand()%256));
			}
			else
			{
				tile->drawPixel(rand()%XMAX, rand()%YMAX, tile->RGB(0, 0, 0));
			}

			wait = rand()%30;
		}
		else
		{
			wait--;
		}
	}
}
