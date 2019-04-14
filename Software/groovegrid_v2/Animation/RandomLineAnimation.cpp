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
			if(rand()%2)
			{
				tile->drawLine(rand()%XMAX, rand()%YMAX, rand()%XMAX, rand()%YMAX, tile->RGB(rand()%256, rand()%256, rand()%256));
			}
			else
			{
				tile->drawLine(rand()%XMAX, rand()%YMAX, rand()%XMAX, rand()%YMAX, tile->RGB(0, 0, 0));
			}

			wait = rand()%animationSpeed;
		}
		else
		{
			wait--;
		}
	}
}
