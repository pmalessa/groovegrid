/*
 * Transition_Boot.cpp
 *
 *  Created on: 28.03.2019
 *      Author: pmale
 */
#include "BootTransition.h"

BootTransition::BootTransition(GridTile* gridTile):GrooveTransition(gridTile)
{
	running = true;
	currentDelay = 0;
	currentColor = CRGBW(0,0,0,0);
	delayFrames = 1000/FRAMERATE_MS;	//1second delay
	state = 0;
	desiredColor = CRGBW(0,0,0,255);	//TODO: replace by persistent storage
	fadeTimer.setTimeStep(5);
}
void BootTransition::run()
{
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
	if(running && frameTimer.isTimeUp())
	{
		switch (state)
		{
		case 0:	//Logo fade in
			tile->writeString(tile->getWidth()>>1,tile->getHeight()>>1,"Gg",currentColor,CRGBW(0,0,0,0),1);
			if(currentColor == desiredColor)
			{
				state = 1;
			}
			break;
		case 1: //delay
			if(currentDelay++ > delayFrames)
			{
				currentColor = CRGBW(0,0,0,0);
				state = 2;
			}
			break;
		case 2: //BG fade in
			tile->writeFilledRect(0,0,tile->getWidth(),tile->getHeight(),currentColor);
			tile->writeString(tile->getWidth()>>1,tile->getHeight()>>1,"Gg",desiredColor,currentColor,1);
			if(currentColor == desiredColor)
			{
				running = false;
			}
			break;
		}
		tile->endWrite();
	}
}

