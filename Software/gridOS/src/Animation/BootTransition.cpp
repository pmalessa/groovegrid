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
	delayFrames = 2000/FRAMERATE_MS;	//1second delay
	tile->endWrite();
	x0 = 0;
	x1 = tile->getWidth()-1;
	state = 0;
}
void BootTransition::run()
{
	if(running && frameTimer.isTimeUp())
	{
		tile->writeString(2,7,"Groovegrid",CRGBW(255,0,255,0),CRGBW(0,0,0,0),1);
		tile->writeFilledRect(x0,0,x1-x0,tile->getHeight(),CRGBW(0,0,0,0));
		tile->endWrite();
		switch (state)
		{
		case 0:	//fade in
			if(x0 < tile->getWidth())
			{
				x0++;
			}
			else
			{
				state = 1;
				x0 = 0;
				x1 = 0;
			}
			break;
		case 1: //fade out
			if(x1 < tile->getWidth())
			{
				x1++;
			}
			else
			{
				running = false;
			}
			break;
		}
	}
}

