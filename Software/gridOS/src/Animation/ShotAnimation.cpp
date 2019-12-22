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
	textColor = CRGB(0xFF0000);
	bgColor = CRGB(0x00FF00);
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
		tile->writeChar(textOffset + textSpacing*0,textHeight,'S',textColor,bgColor,textSize);
		tile->writeChar(textOffset + textSpacing*1,textHeight,'H',textColor,bgColor,textSize);
		tile->writeChar(textOffset + textSpacing*2,textHeight,'O',textColor,bgColor,textSize);
		tile->writeChar(textOffset + textSpacing*3,textHeight,'T',textColor,bgColor,textSize);
		tile->endWrite();
	}
	if(animationTimer.isTimeUp())
	{
		std::swap(textColor,bgColor);
	}
}