/*
 * RandomPixelAnimation.cpp
 *
 *  Created on: 01.04.2019
 *      Author: pmale
 */

#include "RandomPixelAnimation.h"

RandomPixelAnimation::RandomPixelAnimation(GridTile* gridTile)
{
	tile = gridTile;
}

RandomPixelAnimation::~RandomPixelAnimation()
{

}

void RandomPixelAnimation::run()
{
	static uint32_t wait = 0;

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
