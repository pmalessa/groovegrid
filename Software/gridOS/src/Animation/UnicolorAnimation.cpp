/*
 * UnicolorAnimation.cpp
 *
 *  Created on: 02.08.2019
 *      Author: pmale
 */

#include "UnicolorAnimation.h"

UnicolorAnimation::UnicolorAnimation(GridTile *tile):GrooveAnimation(tile)
{
}
UnicolorAnimation::~UnicolorAnimation()
{

}

void UnicolorAnimation::start()
{

}

void UnicolorAnimation::stop()
{

}

void UnicolorAnimation::run()
{
	if(frameTimer.isTimeUp())
	{
		tile->writeFilledRect(0,0,tile->getWidth()-1,tile->getHeight()-1,CRGBW(0, 0, 0, 255));
		tile->writePixel(tile->getWidth()-1,tile->getHeight()-1,CRGBW(0, 0, 0, 255));
		tile->endWrite();
	}
}