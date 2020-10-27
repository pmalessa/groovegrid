/*
 * ShotAnimation.cpp
 *
 *  Created on: 02.08.2019
 *      Author: pmale
 */

#include "ShotAnimation.h"

ShotAnimation::ShotAnimation(GridTile* gridTile):GrooveAnimation(gridTile)
{
	animationSpeed = 15;
	textSize = 0;
	textHeight = tile->getHeight()-1;
	textColor = CRGBW(255,0,0,0);
	bgColor = CRGBW(0,255,0,0);
	textOffset = 2;
	textSpacing = 8;
	animationTimer.setTimeStep(FRAMERATE_MS*animationSpeed);
}

ShotAnimation::~ShotAnimation()
{
}

void ShotAnimation::run()
{
	if(frameTimer.isTimeUp())
	{
		tile->fillScreenBuffer(bgColor);
		tile->writeString(6,10,"SHOT",textColor,bgColor,2);
		tile->endWrite();
	}
	if(animationTimer.isTimeUp())
	{
		std::swap(textColor,bgColor);
	}
}