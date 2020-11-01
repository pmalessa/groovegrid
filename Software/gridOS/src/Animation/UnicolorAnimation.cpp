/*
 * UnicolorAnimation.cpp
 *
 *  Created on: 02.08.2019
 *      Author: pmale
 */

#include "UnicolorAnimation.h"

UnicolorAnimation::UnicolorAnimation(GridTile *tile):GrooveAnimation(tile)
{
	//currentColor = CRGBW(0,0,0,0);
	desiredColor = CRGBW(0,0,0,255);
	fadeTimer.setTimeStep(5);
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
	/*
	if(fadeTimer.isTimeUp())
	{
		if(currentColor.r < desiredColor.r)
		{
			currentColor.r++;
		}
		if(currentColor.g < desiredColor.g)
		{
			currentColor.g++;
		}
		if(currentColor.b < desiredColor.b)
		{
			currentColor.b++;
		}
		if(currentColor.w < desiredColor.w)
		{
			currentColor.w++;
		}
	}
	*/
	if(frameTimer.isTimeUp())
	{
		tile->writeFilledRect(0,0,tile->getWidth(),tile->getHeight(),desiredColor);
		tile->endWrite();
	}
}