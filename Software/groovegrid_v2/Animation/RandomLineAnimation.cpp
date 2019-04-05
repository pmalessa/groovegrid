/*
 * RandomPixelAnimation.cpp
 *
 *  Created on: 01.04.2019
 *      Author: pmale
 */

#include "RandomLineAnimation.h"
#include "../driver/Timer.h"

RandomLineAnimation::RandomLineAnimation(GridTile* gridTile)
{
	tile = gridTile;
	animationSpeed = 5;
	animationStartTime = 0;
}

RandomLineAnimation::~RandomLineAnimation()
{

}

void RandomLineAnimation::run()
{
	static uint32_t wait = 0;
	if(Timer::getMillis()-animationStartTime > animationSpeed)
	{
		animationStartTime = Timer::getMillis();
		if(wait == 0)
		{
			if(rand()%2)
			{
				tile->drawLine(rand()%XMAX, rand()%YMAX, rand()%XMAX, rand()%YMAX, tile->RGB(rand()%256, rand()%256, rand()%256));
			}
			else
			{
				tile->drawLine(rand()%XMAX, rand()%YMAX, rand()%XMAX, rand()%YMAX, tile->RGB(0, 0, 0));
			}

			wait = rand()%30;
		}
		else
		{
			wait--;
		}
	}
}
