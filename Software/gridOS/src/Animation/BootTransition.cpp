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
	tile->drawString(2,7,"Groovegrid",CRGB(0xFF00FF),CRGB(0x000000),1);
	tile->endWrite();
}
void BootTransition::run()
{
	if(running && frameTimer.isTimeUp())
	{
		delayFrames--;
	}
	if(delayFrames == 0)
	{
		running = false;
	}
}

